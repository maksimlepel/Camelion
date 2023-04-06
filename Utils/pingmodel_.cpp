#include <QString>
#include <QDebug>
#include <QTextCodec>
#include <cctype>
#include "PingModel.h"

PingModel::PingModel(QObject *parent) :
    QObject(parent), running(false)
{
    ping = new QProcess(this);
    connect(ping, SIGNAL(started()), this, SLOT(verifyStatus()));
    connect(ping, SIGNAL(finished(int)), this, SLOT(readResult()));
//    ping->setProcessChannelMode(QProcess::MergedChannels);
}

PingModel::~PingModel(){
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
    int i_ping = 0;

    //QString str = "asd34sasd1f";

    qb_new.append(ping->readLine());
    //char *str_3 = qb_new.data();

    qb_new.remove(0, 35);
    qb_new.remove(9, qb_new.size());
    //char *c_ping = qb_new.data();
    //i_ping = atoi(c_ping);

    str = QString::fromUtf8(qb_new).toLocal8Bit().data();

    for (int var = 0; var < str.length(); ++var) {
        //bool ok;
        if (str.at(var).isDigit()){
            int digit = str.at(var).digitValue();
            i_ping = i_ping * 10 + digit;
            qb_new_my.append(i_ping);
        }
    }

    qb_new.clear();
    qb_new_my.clear();

    ping_bullet(i_ping);
}

int PingModel::ping_bullet(int i_p){
    QString res;
    if(i_p > 0 && i_p <= 100)  res = "100%";
    if(i_p > 100 && i_p <= 200)  res = "80%";
    if(i_p > 200 && i_p <= 300)  res = "60%";
    if(i_p > 300 && i_p <= 350)  res = "40%";
    if(i_p > 350 && i_p <= 400 )  res = "20%";
    if(i_p == 0 || i_p > 400 )  res = "0%";
    qDebug() << "ping = " << i_p << " - " << res;
    return i_p;
}

void PingModel::start_command(){
    if(ping){
        QString command = "ping";
        QStringList args;
        //args << "-w" <<  "3" <<  "www.onliner.by";
        args << "192.168.52.11" << "-t";
        ping->start(command, args);
        ping->waitForStarted(100);
        running = true;
        ping->waitForFinished(15000000);
    }
}

bool PingModel::is_running(){
    return running;
}

bool PingModel::finished(){
    return ping->atEnd();
}
