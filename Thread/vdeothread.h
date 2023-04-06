#ifndef VDEOTHREAD_H
#define VDEOTHREAD_H

#include "Thread/ThreadPrototype.h"

class VdeoThread: public ThreadPrototype
{
    Q_OBJECT
public:
    explicit VdeoThread(int numPort, QObject *parent);
    ~VdeoThread();

protected:
    virtual void mainFunction();

private:
 //   QTimer *timer;

public  slots:
  //  void sendInfo();
  //  void doIt(QString str);

signals:
    //void changeSendingInteval(int interval);
    //void doItAgain(QString str);
};

#endif // VDEOTHREAD_H
