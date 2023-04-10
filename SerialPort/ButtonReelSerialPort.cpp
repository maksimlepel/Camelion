#include "ButtonReelSerialPort.h"
#include <QDebug>

ButtonReelSerialPort::ButtonReelSerialPort()
{

    serial = new QSerialPort(this);
    serial->setPortName("COM6");
    serial->open(QSerialPort::OpenModeFlag::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::Parity::NoParity);
    serial->setDataBits(QSerialPort::DataBits::Data8);
    serial->setStopBits(QSerialPort::StopBits::OneStop);

    connect(serial, SIGNAL(readyRead()), this, SLOT(serialReadyRead()));

}



void ButtonReelSerialPort::serialReadyRead(){
    QByteArray data = serial->readAll();
    emit sendButtonReel(data);
    data.clear();
}
