#include "FuncUtils.h"
//#define _WIN32_WINNT 0x0500 //RIGHT
#include "Windows.h"
//#define _WIN32_WINNT 0x0500 //WRONG
//#include <winuser.h>
#include <WinUser.h>
#include <QPixmap>
#include <QDebug>


namespace FuncUtils {

QString getBackgroundColorStr(QWidget *bt)
{
    QString str = "QWidget#" + bt->objectName() + "{ background-color: " + ConstInfo::BUTTON_COLOR + "}" ;
    return str;
}

QString getQDialogColorStr()
{
    QString str = "QDialog{ background-color: "  + ConstInfo::BUTTON_COLOR + " }";
    return str;
}

QString getQLabelColorStr(ConstInfo::TypeMessage type)
{
    QString str;
    switch (type) {
    case ConstInfo::Error:
        str = "QLabel{ background-color: "  + ConstInfo::ERROR_MESSAGE_COLOR + " }";
        break;
    case ConstInfo::Warning:
        str = "QLabel{ background-color: "  + ConstInfo::WARNING_MESSAGE_COLOR + " }";
        break;
    case ConstInfo::Info:
        str = "QLabel{ background-color: "  + ConstInfo::INFO_MESSAGE_COLOR + " }";
        break;
    }
    return str;
}

QString getQPushButtonColorStr(QString color)
{
    QString str = "QPushButton{ background-color: "  + color + " }";
    return str;
}

qint8 twoIntsToByte(int valueL,int valueIR)
{
    // сцепливает два значения int в один байт по полбайта каждое
    int ii = valueL *16+valueIR;
    qint8 res = (qint8) ii;
    return res;
}

qint8 JSVal2RobotVal(float axisVal, double speedMode)
{
    //qDebug() << "SPEED : " << axisVal;
    int result = 0;
//    float val = -1 * axisVal; // инвертируем, т.к. получаем от джойстика обратные значения
    /*if (axisVal < 0){
        result = (axisVal / ConstInfo::AXIS_MIN_VAL* 127 * speedMode) * 0.4;
    }
    else
    {
        result = (axisVal / ConstInfo::AXIS_MAX_VAL  * 127 * speedMode) * 0.4;
    }

    if (abs(result) <= ConstInfo::SENSIVITY ) result = 0;
    // УСТАНОВИТЬ ФОРМУЛУ ДЛЯ РЕЖИМА БЫСТРО  (x/12) в квадрате  // kun
    if (speedMode>ConstInfo::SLOW_MODE)
    {
         // Режим быстро
        if (result < 0) result=-((result/12)*(result/12))*2.5;
        else result=(result/12)*(result/12)*2.5;
         //
    }
    if (result > 48) result = 48;
    if (result < -48) result = -48;*/
    result = axisVal;
    return (qint8)result;
}

QByteArray intToByteArray(int i)
{
    QByteArray ba;
    char a = (i >> 24) & 0XFF;
    char b = (i >> 16) & 0XFF;
    char c = (i >> 8) & 0XFF;
    char d =  i & 0XFF;
    ba.append(a);
    ba.append(b);
    ba.append(c);
    ba.append(d);

    return ba;
}

QByteArray shortToByteArray(short i)
{
    QByteArray ba;
    char c = (i >> 8) & 0XFF;
    char d =  i & 0XFF;
    ba.append(c);
    ba.append(d);

    return ba;
}

short byteArrayToShort(QByteArray ba)
{

    short value = (byte)ba[0];
    value = (value << 8) |(byte) ba[1];
    return value;
}


void pressKey(int keyCode)
{
    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    ip.ki.wVk = keyCode;
    ip.ki.dwFlags = 0; // 0 for key press

    SendInput(1, &ip, sizeof(INPUT));
    // Release
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}

void ledLightsOFF()
{
    // гасим все лампочки
    // если световая индикация включена, то выключаем ее
    if (GetKeyState(VK_CAPITAL) == 1)
    {
       FuncUtils::pressKey(VK_CAPITAL);
    }
    if (GetKeyState(VK_SCROLL) == 1)
    {
       FuncUtils::pressKey(VK_SCROLL);
    }
    if (GetKeyState(VK_NUMLOCK) == 1)
    {
       FuncUtils::pressKey(VK_NUMLOCK);
    }
}

QByteArray byteToTwoByte(qint8 b)
{
    QByteArray ba;
    ba.resize(2);

    ba[0] = (byte) (b&(0xf0))/16;
    ba[1]  = (byte) b&(0x0f);

    return ba;
}

void setLightImage(QLabel *lb, int lightLevel)
{
    qDebug() << "Должны менять иконку!";
    switch (lightLevel) {
    case 0:
        lb->setPixmap(QPixmap(":/Resources/light_level_0"));
        break;
    case 1:
        lb->setPixmap(QPixmap(":/Resources/light_level_1"));
        break;
    case 2:
        lb->setPixmap(QPixmap(":/Resources/light_level_2"));
        break;
    case 3:
        lb->setPixmap(QPixmap(":/Resources/light_level_3"));
        break;
    case 4:
        lb->setPixmap(QPixmap(":/Resources/light_level_4"));
        break;
    default:
        break;
    }
}

void setLightIRImage(QLabel *lb, int lightLevel)
{
    switch (lightLevel) {
    case 0:
        lb->setPixmap(QPixmap(":/Resources/light_level_0"));
        break;
    case 1:
        lb->setPixmap(QPixmap(":/Resources/light_level_1_ir"));
        break;
    case 2:
        lb->setPixmap(QPixmap(":/Resources/light_level_2_ir"));
        break;
    case 3:
        lb->setPixmap(QPixmap(":/Resources/light_level_3_ir"));
        break;
    case 4:
        lb->setPixmap(QPixmap(":/Resources/light_level_4_ir"));
        break;
    default:
        break;
    }
}

}
