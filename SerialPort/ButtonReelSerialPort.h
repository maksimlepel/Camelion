#ifndef BUTTONREELSERIALPORT_H
#define BUTTONREELSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class ButtonReelSerialPort: public QObject
{
    Q_OBJECT
public:
    ButtonReelSerialPort();


private:
    QSerialPort* serial;
    QSerialPortInfo* serialInfo;

private slots:
    void serialReadyRead();

signals:
    void sendButtonReel(QByteArray data);
};

#endif // BUTTONREELSERIALPORT_H
