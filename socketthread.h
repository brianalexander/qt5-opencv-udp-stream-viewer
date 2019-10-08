#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QThread>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "opencv2/opencv.hpp"

class SocketThread : public QThread
{
    Q_OBJECT

public:
    void run(void);

signals:
    void frameReady(cv::Mat frame);
};

#endif // SOCKETTHREAD_H
