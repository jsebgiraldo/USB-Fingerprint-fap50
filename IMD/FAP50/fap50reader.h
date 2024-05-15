#ifndef FAP50READER_H
#define FAP50READER_H

#include "IMD/FAP50/InterfaceImdFap50Method.h"
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <QByteArray>
#include <windows.h>
#include <QTimer>

class Fap50Reader : public QObject
{
    Q_OBJECT

Q_SIGNALS:
    void sig_ImageReady(const QPixmap &pixmap);
public:
    Fap50Reader();

    bool Detect();
    bool Connect();

    void doWork();

    void show_image(ImageStatus img_status);

    void onTimer();
    bool get_flat_finger();

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
    QThread* thread = new QThread;
    QMutex m_mutex;
    QQueue<QString> m_queue;
    QWaitCondition m_condition;
private:
    bool m_stop;
};

#endif // FAP50READER_H
