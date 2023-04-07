#ifndef FUNCUTILS_H
#define FUNCUTILS_H

#include <QWidget>
#include <QString>
#include <QLabel>

#include "ConstInfo.h"

namespace FuncUtils {
    QString getBackgroundColorStr(QWidget *bt);

    QString getQDialogColorStr();

    QString getQLabelColorStr(ConstInfo::TypeMessage type);

    qint8 twoIntsToByte(int valueL,int valueIR) ;

    QString getQPushButtonColorStr(QString color);

    qint8 JSVal2RobotVal(float axisVal, double speedMode);

    QByteArray intToByteArray(int i);

    QByteArray shortToByteArray(short i);

    short byteArrayToShort(QByteArray ba);

    void pressKey(int keyCode);

    void ledLightsOFF();

    QByteArray byteToTwoByte(qint8 b);

    void setLightImage(QLabel *lb, int lightLevel);

    void setLightIRImage(QLabel *lb, int lightLevel);

    //Convert input array in decimal number
    qint8 convertToDecimal(QByteArray array);

}

#endif // FUNCUTILS_H
