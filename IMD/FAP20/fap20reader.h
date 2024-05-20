#ifndef FAP20READER_H
#define FAP20READER_H

#include "IMD/common/devicediscover.h"
#include "fap20controller.h"
#include "IMD/common/modes.h"

#include "fingerprint.h"
#include "nfiq2dll.h"
#include "qdebug.h"

#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QByteArray>
#include <windows.h>
#include <QTimer>

#define TEMPLATE_SIZE  1024


class Fap20Reader : public QObject
{
    Q_OBJECT


Q_SIGNALS:
    void worker_response(const QString &data);
    void sig_ImageReady(const Fingerprint &Finger);


public slots:


    void enqueueMessage(const QString &message) {
        QMutexLocker locker(&m_mutex);
        m_queue.enqueue(message);
        m_condition.wakeOne();
    }

    void stop() {
        QMutexLocker locker(&m_mutex);
        m_stop = true;
        m_condition.wakeOne();
    }

public:
    e_capture_modes CaptureMode;
    int Fingers;
    bool IsCaptureForced;
    bool IsCapturing;
    void doWork();

    Fap20Reader();

    bool Detect();
    bool Connect();
    bool Disconnect();

    void Start(e_capture_modes mode,int fingers);
    void Stop();

    void Test();

    Fingerprint GetImage(Fingerprint::ImageQuality quality);
    std::byte* GenerateTemplate(Fingerprint fingerprint);

public:
    bool IsConnected = false;
    bool m_stop;
    bool bAutoCapture = false;

private:
    DeviceDiscover *Discover = nullptr;
    Fap20Controller *Controller = nullptr;
    Fingerprint * Finger = nullptr;
    Nfiq2Dll * Nfiq2 = nullptr;
    QByteArray fingerprintTemplate;

    QThread* thread = new QThread;
    QMutex m_mutex;
    QQueue<QString> m_queue;
    QWaitCondition m_condition;

    QElapsedTimer stopwatch;
    BYTE RefBuf[TEMPLATE_SIZE];

    void CaptureLiveFingerprint();
    void CaptureAutoFingerprint();
    void EnrollFingerprint();

private slots:

};

#endif // FAP20READER_H
