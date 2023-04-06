#ifndef SENDINGTIMER_H
#define SENDINGTIMER_H

#include <QObject>
#include <QTimer>
#include <QString>

class SendingTimer : public QObject
{
   Q_OBJECT
public:
    SendingTimer(QObject* parent = 0);
    ~SendingTimer();

private:
    QTimer *timer;
    int i;

public slots:
    void changeSendingInteval(int interval);
    void stop();
    void start();
    void sending();

signals:
    void doIt(QString str);
};

#endif // SENDINGTIMER_H
