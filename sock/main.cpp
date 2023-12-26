#include "widget.h"

#include <QApplication>
#include <QLocalSocket>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString hyprInst = QString::fromUtf8(qgetenv("HYPRLAND_INSTANCE_SIGNATURE"));
    QString hyprSocket1 = "/tmp/hypr/" + hyprInst + "/.socket.sock";
    QString hyprSocket2 = "/tmp/hypr/" + hyprInst + "/.socket2.sock";

    QLocalSocket localsock1, localsock2;
    localsock1.connectToServer(hyprSocket1);
    localsock2.connectToServer(hyprSocket2);

    if (!localsock1.waitForConnected()) {
        qDebug() << "error connecting to socket1...";
        return 1;
    }

    if (!localsock2.waitForConnected()) {
        qDebug() << "error connecting to socket2...";
        return 1;
    }

    qDebug() << "connected...";

    QByteArray message = "dispatch workspace 1";
    qint64 bytesWritten = localsock1.write(message);

    if (bytesWritten == -1) {
        qDebug() << "error writing to the socket...";
        return 1;
    }

    localsock1.flush();

    while (localsock2.state() == QLocalSocket::ConnectedState) {
        if (localsock2.waitForReadyRead()) {
            QByteArray data = localsock2.readAll();
            qDebug() << "received this:\n" << data;
        }
    }

    localsock1.close();
    localsock2.close();

    //Widget w;
    //w.show();
    return a.exec();
}
