#include "mainwindow.h"
#include "receiver.h"
#include "socketthread.h"

#include <QUdpSocket>
#include <QApplication>

Q_DECLARE_METATYPE(cv::Mat)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<cv::Mat>();

    MainWindow mainWindow;
    SocketThread socketThread;

    QObject::connect(&socketThread, &SocketThread::frameReady, &mainWindow, &MainWindow::updateFrame);

    socketThread.start();
    mainWindow.show();

    return a.exec();
}
