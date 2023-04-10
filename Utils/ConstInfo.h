#ifndef CONSTINFO_H
#define CONSTINFO_H

#include <QString>

namespace ConstInfo {

    const int FRONT_LIGHT = 0;
    const int REAR_LIGHT = 1;
    const int ARM1_LIGHT = 2;
    const int ARM2_LIGHT = 3;
    const int ELBOW_LIGHT = 4;

    //
    const int ARMBASE = 0;
    const int ARM123 = 1;
    const int ARM234 = 2;
    const int ARM345 = 3;
    const int HEEL = 4;
    const int TANGAGE = 5;


    const int PID_JOYSTICK = 259; // PID джойстика
    const int RUN_THREAD = 1000;
    const int START_THREAD = 1001;
    const int INTERRUPT_THREAD = 1002;

    // режимы движения "зайчик/черепашка"
    const double FAST_MODE = 1;
    const double SLOW_MODE = 0.5;


    // коды кнопок
    const short BUT_MOVE = 2;
    const short BUT_ARM = 0;
    const short BUT_SHOT_RIGHT = 3;
    const short BUT_SHOT_LEFT = 1;

    // коды осей движения
    const short AXIS_FORWARD_BACKWARD = 4;    //0;
    const short AXIS_LEFT_RIGHT = 5;          //1;
    const short AXIS_ROTATION = 3;            //2;

    // коды осей управления рукой
    const short AXIS_ARM_FORWARD_BACKWARD = 1;//3;
    const short AXIS_ARM_LEFT_RIGHT = 0;      //4;
    const short AXIS_ARM_ROTATION = 2;        //5;

    // Наименования кнопок, созданных динамически
    const QString  NAME_BUT_ARM123="Башня+плечо+ локоть";
    const QString  NAME_BUT_ARM234="Плечо+локоть +запястье";
    const QString  NAME_BUT_ARM456="Запястье+вращ.захв.+захв.";
    const QString  NAME_BUT_FAST="Быстро";
    const QString  NAME_BUT_SLOW="Медленно";
    const QString  NAME_BUT_STOP="Стоп";


    const QString IP_ADDRESS = "169.254.181.248";
   // const QString IP_ADDRESS = "10.7.7.124";
    const QString PATH_VIDEO="/teststream";
    const QString PATH_VIDEO2="/temp";

    //const QString IP_ADDRESS = "10.7.7.56";
//    const QString PATH_VIDEO="/str";
//    const QString PATH_VIDEO2="/temp";

    const int PORT_SEND_MSG = 5210; // номер порта для базовых команд роботу
    const int PORT_SEND_MSG_AUDIO = 5212; // номер порта для базовых команд роботу
    const int PORT_RECEIVE_MSG = 5211; // номер порта для прослушивания сигналов от робота

    const int PORT_VIDEO = 8554; // порт для трансляции видео
    const QString DIR_VIDEOS = "d:/Videos/";
    const QString DIR_PHOTOS = "d:/Photo/";

    const int NETWORK_UNREACHABLE = 1;
    const int NETWORK_REACHABLE = 0;

    // ИДЕНТИФИКАТОРЫ УСТРОЙСТВ
    //-------------------------------------------------------------
    const int ID_ENGINE = 1; // для опытного образца в соответствии с протоколом
    //-------------------------------------------------------------

    // ИДЕНТИФИКАТОРЫ ПАКЕТОВ (в соответствии с протоколом)
    //-------------------------------------------------------------
   const qint8 PKG_TYPE_TECH_INFO = (qint8) 0x01; // инф-ция о составе тех. средств робота
   const qint8 PKG_TYPE_INTERFACE_SETTINGS = (qint8) 0x02; //настройка параметров интерфейса
   const qint8 PKG_TYPE_MOBILE_PLATFORM = (qint8) 0xb1; // базовые команды для модулей моб. платформы
   const qint8 PKG_TYPE_MANIPULATOR = (qint8) 0xb2; // базовые команды для модулей манипулятора
    //-------------------------------------------------------------

    // КОМАНДЫ ПАКЕТОВ (в соответствии с протоколом)
    //-------------------------------------------------------------
    const qint8 NO_COMAND = (qint8) 0xFF;

    const int TIME_PERIOD_DEFAULT = 5000;  // период отправки пакетов роботу по умолчанию (редко)
    const int TIME_PERIOD = 200;           // период отправки пакетов роботу при управлении движением или рукой (быстро)
    //-------------------------------------------------------------

    const int PKG_MOTION_SIZE = 15;
    const int PKG_MOTION_SIZE_ARM = 16;


    const QString BUTTON_COLOR = "rgb(166, 232, 253);"; // Цвет подсветки кнопок
    const QString NO_COLOR = "/* empty stylesheet */\n"; // очистка установленного стиля

    const QString ERROR_MESSAGE_COLOR = "rgb(242, 85, 85);"; // Цвет сообщений об ошибке
    const QString WARNING_MESSAGE_COLOR = "rgb(239, 225, 105);"; // Цвет предупреждений
    const QString INFO_MESSAGE_COLOR = "rgb(60, 203, 39);"; // Цвет информационных сообщений
    const QString RECORD_COLOR ="green; "; //Цвет текста ЗАПИСЬ ВИДЕО
    const QString WORK_COLOR ="blue; "; //Цвет текста ЗАПИСЬ ВИДЕО
    const QString NOWORK_COLOR ="black; "; //Цвет текста ЗАПИСЬ ВИДЕО
    const QString ERROR_COLOR ="red; "; //Цвет текста ЗАПИСЬ ВИДЕО


    const int SHOW_MESSAGE_TIME = 3000; // время показа сообщений пользователю в информационной строке

    enum TypeMessage {Info, Warning, Error}; // тип сообщения

    const int AXIS_MAX_VAL = 26360; // максимальное значение оси джойстика
    const int AXIS_MIN_VAL = 24400; // минимальное значение оси джойстика

    const int SENSIVITY = 15; // порог чувствительности джойстка

    const QString CAMERA_1 = "ФРОНТАЛЬНАЯ\n КАМЕРА";
    const QString CAMERA_2 = "ЗАДНЯЯ\n КАМЕРА";
    const QString CAMERA_3 = "КАМЕРА\n НА РУКЕ";
    const QString CAMERA_4 = "ОБЗОРНАЯ \n КАМЕРА";
    const QString CAMERA_5 = "ВЫБЕРИТЕ\n КАМЕРУ!!!";
    const QString CAMERA_6 = "6 камера";

}


#endif // CONSTINFO_H
