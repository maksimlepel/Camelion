#include <QObject>
#include "SendingTimer.h"
#include "Utils/ConstInfo.h"
#include <QDebug>
#include <QDebug>



SendingTimer::SendingTimer(QObject* parent)

{
    i = 0;
    qDebug() << "SendingTimer setParent ";
    this->setParent(parent);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(sending()));
    timer->setInterval(ConstInfo::TIME_PERIOD);
}

SendingTimer::~SendingTimer()
{
    timer->stop();
    delete timer;
}


void SendingTimer::changeSendingInteval(int interval)
{
    i = 0;
    timer->setInterval(interval);
    sending();
}


void SendingTimer::sending()
{
    QString str = QString::number(timer->interval()) + " " + QString::number(i);
    i++;
    emit doIt(str);
}

void SendingTimer::stop()
{
    timer->stop();
}

void SendingTimer::start()
{
    timer->start();
}
