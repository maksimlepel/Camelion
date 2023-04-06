#ifndef THREADPROTOTYPE_H
#define THREADPROTOTYPE_H


#include <QThread>
#include <QTcpSocket>
#include "Utils/ConstInfo.h"

class ThreadPrototype: public QThread
{
    Q_OBJECT
public:
    explicit ThreadPrototype(int numPort, QObject *parent);
    ~ThreadPrototype();

    void run();
    QTcpSocket * socket;
    void reconnect();

public slots:
    void slotError(QAbstractSocket::SocketError err);
    void stateChanged(QAbstractSocket::SocketState st);
    void connected();

protected:
    int numPort;

    bool isCreated;

    void createSocket();
    //QTcpSocket * socket;

    virtual void mainFunction(){};

signals:

    void message(QString message, ConstInfo::TypeMessage typeMessage);
    void errorConnection(int port); // разорвалось соединение с роботом
    void isConnected(int port); // соединение с роботом установлено

};

//Q_DECLARE_METATYPE(QAbstractSocket::SocketState);

#endif // THREADPROTOTYPE_H
