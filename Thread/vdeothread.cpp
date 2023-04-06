#include "vdeothread.h"

VdeoThread::VdeoThread(int numPort, QObject *parent)
    :ThreadPrototype(numPort, parent)
{
    moveToThread(this);
}

void VdeoThread::mainFunction()
{
    createSocket();
}

VdeoThread::~VdeoThread()
{

}

