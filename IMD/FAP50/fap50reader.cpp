#include "Fap50reader.h"
#include "InterfaceImdFap50Method.h"
#include "mainwindow.h"
#include "qdebug.h"
#include "qimage.h"
#include "qlogging.h"
#include "qmessagebox.h"
#include "qpixmap.h"
#include <QThread>

Fap50Reader::Fap50Reader()
{
    m_stop = false;

    moveToThread(thread);
    QObject::connect(thread, &QThread::started, this, &Fap50Reader::Thread);
    QObject::connect(thread, &QThread::finished, this, &Fap50Reader::deleteLater);
    thread->start();
}

void Fap50Reader::Thread() {

    qDebug() << "Thread Start";

    QMap<QString, E_GUI_SHOW_MODE> gui_show_map;
    gui_show_map["GUI_SHOW_MODE_FLAT"] = GUI_SHOW_MODE_FLAT;
    gui_show_map["GUI_SHOW_MODE_ROLL"] = GUI_SHOW_MODE_ROLL;

    QMap<QString, E_FINGER_POSITION> finger_position_map;
    finger_position_map["FINGER_POSITION_RIGHT_FOUR"] = FINGER_POSITION_RIGHT_FOUR;
    finger_position_map["FINGER_POSITION_LEFT_FOUR"] = FINGER_POSITION_LEFT_FOUR;
    finger_position_map["FINGER_POSITION_BOTH_THUMBS"] = FINGER_POSITION_BOTH_THUMBS;

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

        QStringList list = message.split("/");

        if(list[0].contains("GET_FINGER"))
            sampling_finger(gui_show_map[list[1]],finger_position_map[list[2]]);


    }
}

void Fap50Reader::show_image(ImageStatus img_status)
{
    emit sig_ImageReady(img_status);
}


bool Fap50Reader::Connect(){
    IMD_RESULT res = device_reset();
    return res == IMD_RLT_SUCCESS ? true : false;
}

bool Fap50Reader::Detect(){
    SystemProperty property = get_system_property();

    qDebug() << property.chip_id;

    return property.chip_id == 48386 ? true : false;;
}

void Fap50Reader::onTimer()
{
    qDebug() << "onTimer";
}

void callback_fap50_event(IMD_RESULT prompt)
{
    QString str;

    switch (prompt) {
    case IMD_RLT_CLAI_TIMEOUT:
        str.append(L"Calibration timeout! ");
        break;
    case IMD_RLT_SCAN_THREAD_START:
        str.append(L"Start scanning ... ");
        break;
    case IMD_RLT_SCAN_THREAD_END:
        str.append(L"Scan thread end. ");
        break;
    case IMD_RLT_CANT_FIND_ANY_DEVICE:
        str.append(L"Can not find any device. Need to re-plugin USB. ");
        break;
    case IMD_RLT_SCAN_THREAD_IDLE_TIMEOUT:
        str.append(L"Scan idle timeout. ");
        break;
    case IMD_RLT_DCMI_IS_STUCK:
        str.append(L"DCMI has stopped with no response. ");
        break;
    case IMD_RLT_DCMI_TOO_FAST:
        str.append(L"DCMI is too fast. ");
        break;
    case IMD_RLT_USB_READ_IMAGE_EXCEPTION:
        str.append(L"USB read image exception. ");
        break;
    case IMD_RLT_USB_READ_IMAGE_TIMEOUT:
        str.append(L"USB read image timeout. ");
        break;
    case IMD_RLT_RESET_DEVICE_FAIL:
        str.append(L"Reset device fail. ");
        break;
    case IMD_RLT_NO_AVAILABLE_IMAGE:
        str.append(L"No images available. ");
        break;
    case IMD_RLT_CHIP_ID_FAIL:
        str.append(L"Chip ID is wrong. ");
        break;
    default:
        str.append(L"Something happened!");
    }

    qDebug() << str;
}

bool Fap50Reader::sampling_finger(E_GUI_SHOW_MODE mode, E_FINGER_POSITION pos)
{

    if (is_scan_busy())
        scan_cancel();

    IMD_RESULT res = scan_start(mode,pos);
    if(res != IMD_RLT_SUCCESS) return FALSE;


    ImageStatus img_status;

    while(is_scan_busy() == true)
    {
        if (get_image_status(&img_status) == IMD_RLT_SUCCESS)
            show_image(img_status);
        if (img_status.is_finger_on == TRUE && img_status.is_flat_done == TRUE)
        {
            qDebug() << "flat done";
            scan_cancel();
        }
    }

    ImageProperty p;
    p.mode = mode;
    p.pos = pos;
    p.this_scan = TRUE;
    res = get_image(p);
    img_status.result = res;

    if(res==IMD_RLT_SUCCESS)
    {
        emit sig_samplingdone(p);
        return TRUE;
    }

    switch (img_status.result) {
    case IMD_RLT_PUT_WRONG_HAND:
        emit sig_wronghand();
        break;

    case IMD_RLT_POOR_QUALITY_AND_CANTACT_IRON:
    case IMD_RLT_POOR_NFIQ_QUALITY:
        emit sig_poorquality();
        break;

    case IMD_RLT_POOR_QUALITY_AND_WRONG_HAND:
        emit sig_wronghand();
        break;
    default:
        break;
    }

    return FALSE;
}

bool Fap50Reader::get_flat_finger(QString mode, QString pos)
{
    enqueueMessage("GET_FINGER/" + mode + "/" + pos);
    return false;
}
