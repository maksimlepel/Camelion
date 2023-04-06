#include "ThreadPrototype.h"
#include <QTest>
//#include "Utils/ConstInfo.h"

//#include <QNetworkConfigurationManager>
//#include <QNetworkAccessManager>


ThreadPrototype::ThreadPrototype(int numPort, QObject *parent)
:QThread()
{
    qRegisterMetaType <QAbstractSocket::SocketState> ("QAbstractSocket::SocketState");
    moveToThread(this);
    this->numPort = numPort;
    isCreated = false;

}

ThreadPrototype::~ThreadPrototype()
{

}

void ThreadPrototype::run()
{    
   // 07.10.205 Kondrat
    createSocket();
    mainFunction(); // основное тело потока
    exec();
}

void ThreadPrototype::createSocket()
{
      socket = new QTcpSocket(this);
      
      connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)) );
      connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
      connect(socket, SIGNAL(connected()), this, SLOT(connected()) );
      qDebug() << QString::number(numPort) << trUtf8(" Try to connect Robot ... ");
      emit message(QString::number(numPort) + trUtf8(" Установка соединения с ПКР ... ") , ConstInfo::Warning);
      socket->connectToHost(ConstInfo::IP_ADDRESS, numPort);
      while (!isCreated) {
          QTest::qWait(5000);
        //isCreated = socket->waitForConnected(1000);
      }
}

void ThreadPrototype::slotError(QAbstractSocket::SocketError err)
{
    emit message(QString::number(numPort) + trUtf8(" Произошла ошибка (") + socket->errorString() + trUtf8( "). Устанавливаем соединение ... "), ConstInfo::Error);
}

void ThreadPrototype::stateChanged(QAbstractSocket::SocketState st)
{
    QString strOut;
    switch (st) {
    case QAbstractSocket::UnconnectedState:
        strOut = "UnconnectedState";
        //isCreated = false;
        emit errorConnection(numPort);
        QTest::qWait(2000);
        qDebug() << "try to reconnect " << numPort;
        socket->connectToHost(ConstInfo::IP_ADDRESS, numPort);
        break;
    case QAbstractSocket::HostLookupState:
        strOut = "HostLookupState";
        break;
    case QAbstractSocket::ConnectingState:
        strOut = "ConnectingState";
        break;
    case QAbstractSocket::ConnectedState:
    {
        switch (numPort) {
            case ConstInfo::PORT_SEND_MSG:
                qDebug() << trUtf8("Connect Sending");
                emit message(trUtf8("Связь на отправку установлена"), ConstInfo::Info);
                break;
            case ConstInfo::PORT_RECEIVE_MSG:
                qDebug() << trUtf8("Connect Receive");
                emit message(trUtf8("Связь на получение установлена"), ConstInfo::Info);
                break;
            case ConstInfo::PORT_VIDEO:
                qDebug() << trUtf8("Connect Video");
                emit message(trUtf8("Связь на видео установлена"), ConstInfo::Info);
                break;
        }
        isCreated = true;
        qDebug() << "emit isConnected " << numPort;
        emit isConnected(numPort);
        strOut = "ConnectedState";

//        QNetworkConfigurationManager m_config;
//       // QNetworkConfiguration q = m_config.defaultConfiguration();
//        foreach (const QNetworkConfiguration& netConfig, m_config.allConfigurations()) {
//            qDebug() << netConfig.name();
//    //      if (netConfig.bearerType() == QNetworkConfiguration::BearerWLAN) {
//    //        // I'm wireless network
//    //        if (netConfig.type() == QNetworkConfiguration::InternetAccessPoint) {
//    //          // I'm access point
//    //        }
//    //      }
//        }

        break;
    }
    case QAbstractSocket::BoundState:
        strOut = "BoundState";
        break;
    case QAbstractSocket::ClosingState:
        strOut = "ClosingState";
        break;
    case QAbstractSocket::ListeningState:
        strOut = "ListeningState";
        break;
    default:
        strOut = "EPIC FAIL";
        break;
    }
}

void ThreadPrototype::connected()
{
    isCreated = true;
}

void ThreadPrototype::reconnect()
{
    if (socket->state() == QAbstractSocket::UnconnectedState){
        socket->stateChanged(QAbstractSocket::UnconnectedState);
    }
}
