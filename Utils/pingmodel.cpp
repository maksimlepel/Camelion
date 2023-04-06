#include <QString>
#include <QDebug>
#include <QTextCodec>
#include <cctype>
#include "PingModel.h"

PingModel::PingModel(QObject *parent) :
    QObject(parent), running(false)
{
    i_ping = 0;
    ping = new QProcess(this);
    connect(ping, SIGNAL(started()), this, SLOT(verifyStatus()));
    connect(ping, SIGNAL(finished(int)), this, SLOT(readResult()));
//    ping->setProcessChannelMode(QProcess::MergedChannels);
}

PingModel::~PingModel(){
    delete ping;
}

void PingModel::verifyStatus(){
    if(ping->isReadable()){
        qDebug() << "read on ...";
        connect(ping, SIGNAL(readyRead()), this, SLOT(readResult()));
        if(ping->canReadLine()){
            qDebug() << "LINE read on ...";
        }
    }else{
        qDebug() << "not able to read ...";
    }
}

void PingModel::readResult(){
   // QTextCodec* codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    QByteArray qb_new;
    QByteArray qb_new_my;
    running = false;
    QString str;
    int i_ping_2 = 0;

    //QString str = "asd34sasd1f";

    qb_new.append(ping->readLine());
   /* QString str_n = QString::fromUtf8(qb_new).toLocal8Bit().data();
    qDebug() << "qb_new : " << str_n;*/

    //char *str_3 = qb_new.data();

    qb_new.remove(0, 39);
    qb_new.remove(8, qb_new.size());
    char *c_ping = qb_new.data();
    //i_ping = atoi(c_ping);

    str = QString::fromUtf8(qb_new).toLocal8Bit().data();

    for (int var = 0; var < str.length(); ++var) {
        //bool ok;
        if (str.at(var).isDigit()){
            int digit = str.at(var).digitValue();
            //if(digit !=0){
                i_ping_2 = i_ping_2 * 10 + digit;
            //}
            qb_new_my.append(i_ping_2);
        }
    }

    if(str.size() != 0){
        //qDebug() << "str : " << trUtf8(c_ping);// str.toLatin1();
    }

    qb_new.clear();
    qb_new_my.clear();

    if(i_ping_2 != 0){
        i_ping = i_ping_2;
    }
    ping_bullet();
    //i_ping = 0;
}

int PingModel::ping_bullet(){
    QString res;
    if(i_ping > 0 && i_ping <= 100)  res = "100%";
    if(i_ping > 100 && i_ping <= 200)  res = "80%";
    if(i_ping > 200 && i_ping <= 300)  res = "60%";
    if(i_ping > 300 && i_ping <= 350)  res = "40%";
    if(i_ping > 350 && i_ping <= 400 )  res = "20%";
    if(i_ping == 0 || i_ping > 400 )  res = "0%";
    //qDebug() << "ping = " << i_ping << " - " << res;
    return i_ping;
}

void PingModel::start_command(){
    if(ping){
        QString command = "ping";
        QStringList args;
        //args << "-w" <<  "3" <<  "www.onliner.by";
        args << "192.168.52.1" << "-t";
        //args << "netsh wlan show all";
        ping->start(command, args);
        ping->waitForStarted(100);
        running = true;
        ping->waitForFinished(500);
    }
}

bool PingModel::is_running(){
    return running;
}

bool PingModel::finished(){
    return ping->atEnd();
}
