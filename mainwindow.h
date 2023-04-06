#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "Joystick/v_joystick_adapter.h"
#include "Thread/ControllerMsgSendThread.h"
#include "Thread/ControllerMsgRcvThread.h"
#include "Thread/videothread.h"
#include "Utils/ControllerState.h"
#include "Utils/ShowMessage.h"
#include "Utils/ConstInfo.h"
#include "Utils/RobotState.h"
#include "Utils/FuncUtils.h"
#include "OgreView/OgreView.h"
#include "Thread/TabletBatteryLevelRequest.h"
#include "Dialogs/dgVLC.h"
#include <QTimer>
#include <QByteArray>
#include <qaudioinput.h>
#include<qaudiooutput.h>
#include <QBuffer>
#include <Utils/PingModel.h>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class QTcpServer;
class QTextEdit;
class QTcpSocket;

Q_DECLARE_METATYPE ( ConstInfo::TypeMessage );

class MainWindow : public QMainWindow
{
    Q_OBJECT

   private:
    int argc;
    char **argv;

public:
    explicit MainWindow(int argc, char **argv, QWidget *parent = 0);
    ~MainWindow();

     QTcpSocket* m_pTcpSocket;

     int convertToDecimal(QByteArray array);
     static int i_U_rob;
     ShowMessage *showMessage2;
     QByteArray prevData;

     // MyServer(int nPort, QWidget* pwgt = 0);
public slots:
     /*bool isOpen();
     bool open(void);
     bool close_port(void);*/
     void errorSerial(QSerialPort::SerialPortError err);

private slots:


    void sendData(QByteArray data);
    void onReadyRead();

    void reconnect();
    //void reconnect_port();

    void readMore();
    //void on_pushButton_clicked();
    //void on_horizontalSlider_valueChanged(int value);
    //void on_pushButton_2_clicked();

    //void updateTime_my_2();
    //virtual void slotNewConnection();
    //void slotReadClient();

    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotSendToServer();
    void slotConnected();

    void updateTime_my();
    void updateTime_my_2();
    void update_progressBar();
    void on_btVideo_clicked();
    void on_btAudio_clicked();
    void on_btLight_clicked();
    void on_btLightIR_clicked();

    void buttonChanged(int id, bool state);
    void axisChanged(int id, int state);
    void on_btRemote_clicked();
    void lightOff(bool isIRLight);
    void setLight(int lightId, int lightValue,bool isIRLight);
    void doItAgain(QString str);
    void onTalk();
    void onListen();
    void errorConnection(int port);
    void isConnected(int port);
    void onRecordVideo();
    void sendToRobot();


    //07.10.2015 Kondrat

    void startVideoStream( VlcMedia  *_media);


    void on_btArm456_clicked();

    void on_btArm234_clicked();

    void on_btArm123_clicked();

    void on_btFast_clicked();

    void on_btSlow_clicked();


    void on_btStopMove_clicked();

    void on_btWindowMode_clicked();

    void changePositionArm(int idArmJoint, int positionValue);
    void changeLight(int idLight, int lightLevel, bool isIRLight);
    void changeBatteryLevelRob(int batteryLevel);
    void changeSignalLevelRob(int signalLevel);
   // void changeFlippers(int angle);

    void on_btCamera_clicked();
    void on_btCamera_2_clicked();

    // tablet battery level
    void setBatteryLevel(int batteryLevel);

    void on_btConnection_clicked();

    void on_btPhoto_clicked();

    void on_btArm_clicked();
    void onOneJoystick();
    void onTwoJoystick();
    void dgSecondVideoShow();
    void dgSecondVideoDelete();
    void receiveMedia2(VlcMedia  * vlcMedia);

    void on_pushButton_clicked();


    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    void initializeAudio();
    void createAudioInput();
    void createAudioOutput();
    int fromHexToDecimal(QByteArray arrayForU2);

    //QTcpServer* m_ptcpServer;

    QTextEdit*  m_ptxt;
    quint16     m_nNextBlockSize;

    int ApplyVolumeToSample(short iSample);
    quint32 listFolder(QString path);

   // int counter;
    //QTimer *timer_1;
    void sendToClient(QTcpSocket* pSocket, const QString& str);

    //QAudioDeviceInfo getAudioDevice(QString& name);
    //void audio_for_tcp(QTcpSocket* pSocket, QByteArray ba, int len);

    QSerialPort *serialPort;
    QTimer* sendTimer;
    QByteArray receiveDataBuffer;
    QByteArray speed(QByteArray qba_XY);
    void lvlAKB_2(QByteArray qbaU);;
    void lvlAKB(QByteArray qbaU);
    static QByteArray qbaForMrk;
    static QByteArray qba_From_Mrk;
    static QByteArray qba_U_MRK;

    QTimer *tmr; //Адресная переменная таймера
    QTimer *tmr_2; //Адресная переменная таймера
    QTimer *tmr_5s; //Адресная переменная таймера

    int counter_my;
    int counter_my_2;
    int counter_btvideo_click;
    int my_lbl;

    QString url_2;

    static QByteArray qbArray_audio;
    static QByteArray qba_cc;

    OgreView *ogreView;

    VJoystickAdapter* joystick;
    ControllerState *contrState;
    RobotState *robotState;
    //FuncUtils *funcUtils;
    ControllerMsgSendThread *cntrlMsgSendThread;
    ControllerMsgRcvThread *cntrlMsgRcvThread;
    ShowMessage *showMessage;
    VideoThread *videoThr;
    PingModel *pingmodel;
    bool isMaximizedStayOnTop;

    VlcInstance *_instance;
    VlcInstance *_instance_2;
    VlcMedia *_media;
    VlcMedia *_media_2;
    VlcMediaPlayer *_player;
    VlcMediaPlayer *_player_2;
//    QString btNameRecord;
    QProcess * m_process;

    TabletBatteryLevelRequest *battLevelThr;

    bool startMedia;
    void joystickInit();

    bool isVideoConnected;
    bool isSendConnected;
    bool isReceiveConnected;
    bool isStop;
    bool isTwoJoystick;//true- 2 джойстика, false-1 джойстик
    MoveMode modeMove;

    dgVLC *dgSecondVideo;
    VlcMedia  *media2;
    void clearColorFilterArmR();        // гасим на экране подсветку кнопок режимов руки справа на экране(2 джойстика)
    void clearColorFilterArm();         // гасим на экране подсветку кнопок режимов руки
    void clearColorFilterMove();        // гасим на экране подсветку кнопок режимов движения
    void changeLightIndicatorArm();     // изменение индикации режимов руки (световой и на экране)
    void changeLightIndicatorMove();    // изменение индикации режимов движения (световой и на экране)
    void setLedLigths();                // установка Led индикации
    void setIndicatorArm();             // установка на экране режимов руки
    void setVisibleRArmButtons(bool visible); // управление отображением кнопок для второго джойстика

    void changesStateButtonMove(); // выполнение слотов  при работе с кнопками пульта ДВИЖЕНИЕ  //kun
    void changesStateButtonArm();  // выполнение слотов  при работе с кнопками пульта РУКА //kun
    void setInterfaceTwoJoystick(bool twoJoystick);

    QAudioDeviceInfo m_Inputdevice;
    QAudioDeviceInfo m_Outputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QIODevice *m_output;
    QByteArray m_buffer;
    int m_iVolume;

signals:

    void message(QString message, ConstInfo::TypeMessage typeMessage);
    void signalErrComPort(QSerialPort::SerialPortError err);
    void send(QByteArray data);

};

#endif // MAINWINDOW_H
