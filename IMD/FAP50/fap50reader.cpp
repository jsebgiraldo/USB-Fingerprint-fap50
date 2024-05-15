#include "Fap50reader.h"
#include "InterfaceImdFap50Method.h"
#include "qdebug.h"
#include "qimage.h"
#include "qlogging.h"
#include "qpixmap.h"
#include <QThread>

Fap50Reader::Fap50Reader()
{
    m_stop = false;

    moveToThread(thread);
    QObject::connect(thread, &QThread::started, this, &Fap50Reader::doWork);
    QObject::connect(thread, &QThread::finished, this, &Fap50Reader::deleteLater);
    thread->start();
}

void Fap50Reader::doWork() {

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

        qDebug() << message;

        if(message.contains("SHOW_FLAT_IMAGE"))
        {
            thread->msleep(1000);
            while(is_scan_busy() == true)
            {
                ImageStatus img_status;
                if (get_image_status(&img_status) == IMD_RLT_SUCCESS)
                    show_image(img_status);
                if (img_status.is_finger_on == FALSE && img_status.is_flat_done == TRUE)
                    scan_cancel();
                thread->msleep(500);
            }


        }

    }
}

void Fap50Reader::show_image(ImageStatus img_status)
{


    ImageProperty img_property;
    get_image(img_property);

    BYTE* img = img_property.img;


    qDebug() << img_status.img << img;

   // emit sig_ImageReady(pixmap);
}


bool Fap50Reader::Detect(){
    IMD_RESULT res = device_reset();
    return res == IMD_RLT_SUCCESS ? true : false;
}

bool Fap50Reader::Connect(){
    SystemProperty property = get_system_property();

    qDebug() << property.chip_id;

    return property.chip_id == 100992006 ? true : false;;
}

void Fap50Reader::onTimer()
{
    qDebug() << "onTimer";
}

bool Fap50Reader::get_flat_finger()
{
    if (is_scan_busy())
        scan_cancel();


    std::vector< E_FINGER_POSITION> fingers = { FINGER_POSITION_RIGHT_RING, FINGER_POSITION_RIGHT_LITTLE,
                                         FINGER_POSITION_LEFT_THUMB, FINGER_POSITION_LEFT_INDEX };
    IMD_RESULT res = scan_fingers_start(GUI_SHOW_MODE_CAPTURE, fingers.data(), fingers.size());

    //IMD_RESULT res = scan_start(GUI_SHOW_MODE_ROLL, FINGER_POSITION_RIGHT_THUMB);

    qDebug() << res << "scan_start(GUI_SHOW_MODE_FLAT, FINGER_POSITION_RIGHT_THUMB)";

    enqueueMessage("SHOW_FLAT_IMAGE");

    return false;
}
