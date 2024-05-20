#include "fap20reader.h"

#include <QMessageBox>
#include <QThread>

Fap20Reader::Fap20Reader() {
    Discover = new DeviceDiscover();
    Controller = new Fap20Controller();
    Finger = new Fingerprint();
    Nfiq2 = new Nfiq2Dll();
    m_stop = false;
}

void Fap20Reader::doWork() {

    qDebug() << "Thread Start";

    while (true) {
        m_mutex.lock();
        if (m_stop) {
            m_mutex.unlock();
            return;
        }
        if (m_queue.isEmpty()) {
            m_condition.wait(&m_mutex);
        }
        QString message = m_queue.dequeue();
        m_mutex.unlock();

        if(message.contains("capture")){
            IsCapturing = true;
            switch (CaptureMode) {
            case e_capture_modes::enrollStart:
                break;
            case e_capture_modes::authenticationStart:
                qDebug() << "CaptureLiveFingerprint";
                CaptureLiveFingerprint();

                break;
            case e_capture_modes::authenticationAuto:
                qDebug() << "CaptureAutoFingerprint";
                CaptureAutoFingerprint();

                break;
            default:
                break;
            }
        }else
        {

        }



    }
}

Fingerprint Fap20Reader::GetImage(Fingerprint::ImageQuality quality)
{
    switch (quality)
    {
    case Fingerprint::Low:
    case Fingerprint::Default:
    case Fingerprint::High:
        Controller->SafeCaptureImage();
        break;
    default:
        break;
    }

    int message = 0;

    while ((message != Fap20Controller::FPM_CAPTURE))
    {
        thread->msleep(250);
        message = Controller->SafeGetWorkMsg();

        qDebug() << message;

        if(message == Fap20Controller::FPM_TIMEOUT)
        {
            Controller->SafeCaptureImage();
        }
    }



    if(message == -1)
    {
        return *new Fingerprint();
    }

    Controller->SafeGetImageSize(&Finger->imgwidth, &Finger->imgheight);
    Controller->SafeGetRawImage(Finger->rawImage, &Finger->rawSize);
    Controller->SafeResetDevice();

    QImage image((const uchar*)Finger->rawImage, Finger->imgwidth, Finger->imgheight, QImage::Format_Grayscale8);
    Finger->pixmap = QPixmap::fromImage(image);


    char *ptr = Nfiq2->ConvertToIntPtr(Finger->rawImage);
    int score = Nfiq2->SafeComputeNfiq2Score(1, (char *)Finger->rawImage, 400*500, Finger->imgwidth, Finger->imgheight, 500);
    Nfiq2->FreeIntPtr(ptr);
    Finger->Score = score > 100 ? 0 : score;

    return *Finger;
}

bool Fap20Reader::Detect(){
    return Discover->GetDevices("VID_2B41&PID_0301");
}

bool Fap20Reader::Connect(){

    if(IsConnected)
    {
        return true;
    }

    Controller->SafeCloseDevice();

    if (Controller->SafeOpenDevice() == 1 && Controller->SafeLinkDevice(0) == 1)
    {
        QByteArray hash;
        Nfiq2->InitNfiq(hash);

        moveToThread(thread);
        QObject::connect(thread, &QThread::started, this, &Fap20Reader::doWork);
        QObject::connect(thread, &QThread::finished, this, &Fap20Reader::deleteLater);
        thread->start();

        IsConnected = true;
    }
    else
    {
        Controller->SafeCloseDevice();
        IsConnected = false;
    }

    return IsConnected;
}

bool Fap20Reader::Disconnect(){
    thread->quit();
    return Controller->SafeCloseDevice();
}

void Fap20Reader::Test()
{
    Controller->SafeCaptureTemplate();
    return;
}

void Fap20Reader::Start(e_capture_modes mode,int fingers)
{

    CaptureMode = mode;
    Fingers = fingers;

    enqueueMessage("capture");

    return;
}

void Fap20Reader::Stop()
{
    IsCapturing = false;
    return;
}

void Fap20Reader::CaptureAutoFingerprint()
{
    while(IsCapturing)
    {
        Fingerprint Image = GetImage(IsCaptureForced ? Fingerprint::High : Fingerprint::Low);
        emit sig_ImageReady(Image);
        Controller->SafeDeviceLedState(0x37, 0x00);
    }
}

void Fap20Reader::CaptureLiveFingerprint()
{
    Fingerprint Image = GetImage(IsCaptureForced ? Fingerprint::High : Fingerprint::Low);
    emit sig_ImageReady(Image);
    Controller->SafeDeviceSoundBeep(Fap20Controller::SOUND_OK);
    Controller->SafeDeviceLedState(0x37, 0x00);
}

std::byte* Fap20Reader::GenerateTemplate(Fingerprint fingerprint)
{
    int size = 1024;
    std::byte* templateBytes = new std::byte[size];
    int templateSize = 0;

    int w = fingerprint.pixmap.size().width();
    int h = fingerprint.pixmap.size().height();

    Controller->SafeCreateTemplate(fingerprint.rawBytes, w, h, templateBytes, &templateSize);
    return templateBytes;
}

