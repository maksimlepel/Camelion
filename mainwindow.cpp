#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTextCodec"
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include "Dialogs/dgAudio.h"
#include "Dialogs/dgLight.h"
#include "Dialogs/dgArm.h"
#include "Utils/FuncUtils.h"
#include "Utils/ConstInfo.h"
#include <windows.h>
#include <QFileDialog>
#include <QInputDialog>
#include <QTest>
#include <QFont>
#include <QMessageBox>
#include <QStringList>
#include <libvlc-qt/include/VLCQtCore/Common.h>
#include <libvlc-qt/include/VLCQtCore/Instance.h>
#include <libvlc-qt/include/VLCQtCore/Media.h>
#include <libvlc-qt/include/VLCQtCore/MediaPlayer.h>
#include <Dialogs/dgCameraMode.h>
#include <QStatusBar>
#include <OgreView/OgreView.h>
#include <Utils/PingModel.h>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QMutex>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

#include <libvlc-qt/include/VLCQtCore/Common.h>
#include <libvlc-qt/include/VLCQtCore/Instance.h>
#include <libvlc-qt/include/VLCQtCore/Media.h>
#include <libvlc-qt/include/VLCQtCore/MediaPlayer.h>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

static int arr_[360];
static int i_U;
static int i_U2;
static int iRecVideo;
static int iPlatf_Monip;

int MainWindow::i_U_rob;
const int BufferSize = 8192;

//static int counter_btvideo_click;
QByteArray MainWindow::qbArray_audio;
QByteArray MainWindow::qba_cc;
QByteArray MainWindow::qbaForMrk;
QByteArray MainWindow::qba_From_Mrk;
//QByteArray MainWindow::qba_U_;
QByteArray MainWindow::qba_U_MRK;
QString str_res_1;
QString str_res_2;
QString str_res_3;
QString str_res_4;
QString str_res_5;
QString str_res_6;
QString name_file_videos;
const QString color_r = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
                 "#EE0000, stop: 0.5 #EE0000, stop: 1 #EE0000)}";
        //border-bottom-rigth-radius: 5px; border-bottom-left-radius: 5px; border: .px solid back;}";
const QString color_y = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
                 "#EEEE00, stop: 0.5 #EEEE00, stop: 1 #EEEE00 )}";
// border-bottom-rigth-radius: 5px; border-bottom-"
                 //"left-radius: 5px; border: .px solid back;}";
const QString color_g = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
                 "#00EE00, stop: 0.5 #00EE00, stop: 1 #00EE00 )}";
                        //border-bottom-rigth-radius: 5px; border-bottom-"
                 //"left-radius: 5px; border: .px solid back;}";
/*QString safe = "QProgressBar::chunk {background: QLinearGradient( x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 "
                 "#78d, stop: 0.4999 #46a, stop: 1 #238 ); border-bottom-rigth-radius: 7px; border-bottom-"
                 "left-radius: 7px; border: 1px solid back;}";*/
const QString deviceId_ = "Микрофон (Realtek High Definition Audio)";

MainWindow::MainWindow(int argc, char **argv, QWidget *parent) :
    QMainWindow(parent),
    m_Inputdevice(QAudioDeviceInfo::defaultInputDevice()),
    m_Outputdevice(QAudioDeviceInfo::defaultOutputDevice()),
    m_audioInput(0),
    m_audioOutput(0),
    m_input(0),
    m_iVolume(0),
    m_buffer(BufferSize, 0),
    m_nNextBlockSize(0),
    serialPort(new QSerialPort),
    sendTimer(new QTimer),
    ui(new Ui::MainWindow),
    _media_2(0),
    joystick(0)
{
    this->argc = argc;
    this->argv = argv;

    QString deviceId = "Микрофон (Realtek High Definition Audio)";

     QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

     m_Inputdevice.deviceName().clear();
     qDebug() << "name input device default : " << m_Inputdevice.deviceName();

    /* m_Inputdevice.deviceName().append(deviceId_);
     qDebug() << "name my input device : " << m_Inputdevice.deviceName();
*/
    counter_my = 0;
    counter_my_2 = 0;
    iRecVideo = 0;
    iPlatf_Monip = 0;
    i_U = 0;
    i_U2 = 0;
    qba_U_MRK[0] = (char) 0x00;

    //ogreView = new OgreView(this);
    //ui->horizontalLayout_2->insertWidget(1, ogreView, 1);
    startMedia = true;
    qba_cc[0] = (char) 0x00;

    ui->setupUi(this);

    sendTimer->setInterval(75);

    //connect(sendTimer,&QTimer::timeout,this,&MainWindow::onSendDataTimeout);
    sendTimer->start();
//    server = new QTcpServer();
//    if(server->listen(QHostAddress::Any, 666)){
//        qDebug() << "Listening...";
//    } else{
//        qDebug() << "Error...";
//    }

    QDir dir(ConstInfo::DIR_VIDEOS);
    qDebug() << "size dir videos = " << listFolder(ConstInfo::DIR_VIDEOS);
    if(listFolder(ConstInfo::DIR_VIDEOS) > 100000){

        switch( QMessageBox::question(
                    this,
                    "Папка с видео занимает больше 100 гигабайт!",
                    "Вы хотите очистить папку с видео?\n(Все записанные видеофайлы в папке C:/Videos будут удалены!" ,
                    QMessageBox::Yes |
                    QMessageBox::No  |
                    QMessageBox::Cancel,

                    QMessageBox::Cancel ) )
        {
          case QMessageBox::Yes:
            dir.removeRecursively();
            break;
          case QMessageBox::No:
            QMessageBox::warning(
                        this,
                        "Предупреждение",
                        "Папка с видео не была очищена!\nЭто может в дальнейшем вызвать проблемы с запуском ПО на устройстве управления!");
            break;
          case QMessageBox::Cancel:
            QMessageBox::warning(
                        this,
                        "Предупреждение",
                        "Папка с видео не была очищена!\nЭто может в дальнейшем вызвать проблемы с запуском ПО на устройстве управления!");
            break;
          default:
            break;
        }
   }

    /*if(dir.exists())
        dir.removeRecursively();*/
    if(!dir.exists())
        QDir().mkdir(ConstInfo::DIR_VIDEOS);

    m_process = new QProcess(this);
    //name_file_videos = ConstInfo::DIR_VIDEOS+"/video-record.avi"; //+QDateTime::currentDateTime().toString("dd-MM-yy_hh-mm-ss")+".avi";
    name_file_videos = ConstInfo::DIR_VIDEOS + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
    qDebug() << "name_file_videos : " << name_file_videos;
    counter_btvideo_click = 0;

    QAudioFormat m_format;
   // initializeAudio();

    ui->lvOtherInfo->setEnabled(false);
    ui->lvOtherInfo->setVisible(false);

    //ui->lvOtherInfo->addColumn("Right Column");
    ui->btConnection->setVisible(true); //ui->btConnection->setVisible(true);
    ui->label_angle->setVisible(false);
    ui->label_angle_2->setVisible(false);
    /*ui->lbPower2->setVisible(false);
    ui->lbPower3->setVisible(false);
    ui->progressBar_pl2->setVisible(false);
    ui->progressBar_rbt->setVisible(false);*/

    statusBar()->addPermanentWidget(ui->label_for_bar);
    statusBar()->addWidget(ui->infoLabel);

    ui->label_for_bar->setStyleSheet("QLabel {color : blue;}");
    //statusBar()->setStyleSheet("*{background-color: grey;}");
    //statusBar()->setLayoutDirection(Qt::RightToLeft);

    //ui->statusBar->addPermanentWidget(ui->label_for_bar);
    statusBar()->setStyleSheet(QString::fromUtf8("background-color: rgb(183, 242, 255);"));

    ui->btVideo->setVisible(true);
    //ui->progressBar_pl1->setValue(80);

    //отключение верхней панели
    Qt::WindowFlags wf = windowFlags();
    wf = Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint;
    setWindowFlags(wf);

    /*Qt::WindowFlags wf = windowFlags();
    wf ^= Qt::WindowStaysOnTopHint;
    wf ^= Qt::FramelessWindowHint;
    setWindowFlags(wf);
    ui->btConnection->setVisible(true);
    show();*/
    isMaximizedStayOnTop = false;
    //isMaximizedStayOnTop = true;

    isVideoConnected = true;
    isSendConnected = false;
    isReceiveConnected = false;
    isStop = false;
    isTwoJoystick = false; //isTwoJoystick = false;

    setVisibleRArmButtons(false);

    QFont font1;
    font1.setWeight(99); // Жирный шрифт

    //ui->lbRecordFile->setStyleSheet("QLabel { color :"+ConstInfo::RECORD_COLOR+" }");
    //ui->lbRecordFile->setFont(font1);
    //ui->lbRecordFile->setText(" ");
    //ui->lbRecordFile->setText(trUtf8("ИДЕТ СОЕДИНЕНИЕ С РОБОТОМ!"));
    //ui->infoLabel->setText(trUtf8("ИДЕТ СОЕДИНЕНИЕ С РОБОТОМ!"));
    //ui->statusBar->addPermanentWidget(ui->lbRecordFile);

    ui->btAudio->setEnabled(false);
    ui->btStopMove->setEnabled(false);
    ui->btFast->setEnabled(false);
    ui->btSlow->setEnabled(false);
    ui->btCamera->setEnabled(false);
    ui->btCamera_2->setEnabled(false);
    ui->btLight->setEnabled(false);
    ui->btLightIR->setEnabled(false);


    //btNameRecord="Видеозапись";

    ui->label_listen->setVisible(false);
    ui->label_talk->setVisible(false);
    ui->label_invers->setVisible(false);
    ui->label_7->setVisible(false);
    ui->label_12->setVisible(false);
    ui->label_radiomodem->setVisible(false);
    ui->label_listen_on->setVisible(false);
    ui->progressBar_pl2->setVisible(false);
    ui->lbPower2->setVisible(false);
    ui->label_signal_level->setVisible(false);
    showMessage2 = new ShowMessage(ui->label_8);


//    ui->label_8->setStyleSheet("QLabel {background-color : blue; color: black;}");
//    QFont font("Times", 28, QFont::Bold);
//    ui->label_8->setFont(font);
//    ui->label_8->setText("ОАО \"АГАТ - системы управления\" - \n"
//                         "управляющая компания холдинга\n"
//                         "\"Геоинформационные системы управления\"\n"
//                         "г. Минск, 2020 г.");

    //ui->horizontalLayout_2->setVisible(false);

  /*  ui->widgetVideo->setGeometry(10, 40, 450, 430);
    ui->widgetVideo_2->setGeometry(470, 40, 440, 430);*/

    //ui->btAudio->setVisible(false);
    //ui->verticalSpacer_7->changeSize(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored);
    //ui->verticalSpacer->changeSize(0, 0, QSizePolicy::Ignored, QSizePolicy::Ignored);


    ui->lvOtherInfo->setVisible(false);
    ui->btArm123->setVisible(false);
    ui->btRArm123->setVisible(false);
    ui->btArm456->setVisible(false);
    ui->btRArm456->setVisible(false);
    ui->btArm234->setVisible(false);
    ui->btArm->setVisible(false);
    ui->btAudio->setVisible(false);
    ui->btCamera_2->setVisible(true);
    ui->btWindowMode->setVisible(false);

    ui->progressBar_pl1->setTextVisible(false);
    ui->progressBar_rbt->setTextVisible(false);

    ui->progressBar_pl1->setValue(99);
    ui->progressBar_rbt->setValue(99);

    qRegisterMetaType<ConstInfo::TypeMessage>( "ConstInfo::TypeMessage" );

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    joystick = new VJoystickAdapter(this);

    dgSecondVideo = NULL;

    contrState = new ControllerState();
    robotState = new RobotState();

    pingmodel = new PingModel();
    //pingmodel->start_command();
    //pingmodel->verifyStatus();
    //pingmodel->readResult();

    //funcUtils = new FuncUtils();
    showMessage = new ShowMessage(ui->infoLabel);
    //showMessage = new ShowMessage(ui->label_for_bar);

    tmr = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    tmr->setInterval(100); // Задаем интервал таймера
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime_my())); // Подключаем сигнал таймера к нашему слоту
    tmr->start(); // Запускаем таймер

    tmr_2 = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    tmr_2->setInterval(900600); // Задаем интервал таймера
    connect(tmr_2, SIGNAL(timeout()), this, SLOT(updateTime_my_2())); // Подключаем сигнал таймера к нашему слоту
    //tmr_2->start(); // Запускаем таймер

    tmr_5s = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    connect(tmr_5s, SIGNAL(timeout()), this, SLOT(update_progressBar()));

    //создаем и запускаем поток на чтение сообщений от робота
//    cntrlMsgRcvThread = new ControllerMsgRcvThread(/*ConstInfo::PORT_RECEIVE_MSG*/ConstInfo::PORT_SEND_MSG, robotState, this);
//    connect(cntrlMsgRcvThread, SIGNAL(changeLight(int, int, bool)), this, SLOT(changeLight(int,int,bool)));
//    connect(cntrlMsgRcvThread, SIGNAL(changePositionArm(int, int)), this, SLOT(changePositionArm(int, int)));
//    connect(cntrlMsgRcvThread, SIGNAL(changeFlippers(int)), this, SLOT(changeFlippers(int)));
//    connect(cntrlMsgRcvThread, SIGNAL(changeBatteryLevelRob(int)), this, SLOT(changeBatteryLevelRob(int)));
//    connect(cntrlMsgRcvThread, SIGNAL(changeSignalLevelRob(int)), this, SLOT(changeSignalLevelRob(int)));
//    connect(cntrlMsgRcvThread, SIGNAL(isConnected(int)), this, SLOT(isConnected(int)));
//    connect(cntrlMsgRcvThread, SIGNAL(errorConnection(int)), this, SLOT(errorConnection(int)));
//    //cntrlMsgRcvThread->setParent(this);
//    cntrlMsgRcvThread->start();

    //создаем и запускаем поток на считываение и отправку состяния пульта управления
    cntrlMsgSendThread = new ControllerMsgSendThread( ConstInfo::PORT_SEND_MSG, robotState, contrState, this);
    connect(cntrlMsgSendThread, SIGNAL(message(QString, ConstInfo::TypeMessage)), showMessage, SLOT(addMessage(QString, ConstInfo::TypeMessage)));
    connect(this, SIGNAL(message(QString, ConstInfo::TypeMessage)), showMessage, SLOT(addMessage(QString, ConstInfo::TypeMessage)));
    connect(cntrlMsgSendThread, SIGNAL(doItAgain(QString)), this, SLOT(doItAgain(QString)));
    connect(cntrlMsgSendThread, SIGNAL(errorConnection(int)), this, SLOT(errorConnection(int)));
    connect(cntrlMsgSendThread, SIGNAL(isConnected(int)), this, SLOT(isConnected(int)));
  //  connect(cntrlMsgSendThread, SIGNAL(changeFlippers(int)), this, SLOT(changeFlippers(int)));
    connect(cntrlMsgSendThread, SIGNAL(changeLight(int, int, bool)), this, SLOT(changeLight(int,int,bool)));
    //connect(cntrlMsgSendThread, SIGNAL(setLight(int, int, bool)), this, SLOT(setLight(int,int,bool)));
    connect(cntrlMsgSendThread, SIGNAL(changePositionArm(int, int)), this, SLOT(changePositionArm(int, int)));
    connect(cntrlMsgSendThread, SIGNAL(changeBatteryLevelRob(int)), this, SLOT(changeBatteryLevelRob(int)));
    connect(cntrlMsgSendThread, SIGNAL(changeSignalLevelRob(int)), this, SLOT(changeSignalLevelRob(int)));

    //cntrlMsgSendThread->setParent(this);
    cntrlMsgSendThread->start();


    joystickInit();
    
    // устанавливаем по молчанию режим скорости - МЕДЛЕННО (черепаха) и
    // режим руки - Башня-плечо-локоть
    contrState->setArmMode(ARM_456); //contrState->setArmMode(ARM_123);
    contrState->setMoveMode(MOVE_SLOW);
    changeLightIndicatorArm();
    changeLightIndicatorMove();
    modeMove = MOVE_SLOW;

    //setLedLigths(); //вкл. и выкл. CAPSLOCK и т.п.

  /*  ogreView->rotateTurret(-90);
    ogreView->rotateFlippers(90);
    ogreView->rotateJoint2(-90);
    ogreView->rotateJoint2(-145);*/

    // Kondrat
    QStringList vlc_args = QStringList() << "--network-caching=0";
//    _instance = new VlcInstance(vlc_args, this);

    /*QStringList args1 = VlcCommon::args();
    for(int i = 0; i < argc; i++){
        args1 << QString(argv[i]);
    }*/
    //args1.append(":network-caching=100");
    //_instance = new VlcInstance(args1, this);
    _instance = new VlcInstance(vlc_args, this);

    //_instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->widgetVideo_2);

    /*QStringList args2 = VlcCommon::args();
    for(int i = 0; i < argc; i++){
        args2 << QString(argv[i]);
    }*/
    //args2.append(":network-caching=100");
    //_instance_2 = new VlcInstance(args2, this);
    _instance_2 = new VlcInstance(vlc_args, this);
    _player_2 = new VlcMediaPlayer(_instance_2);
    _player_2->setVideoWidget(ui->widgetVideo);

    ui->widgetVideo->setMediaPlayer(_player_2);
    url_2 = "rtsp://192.168.52.1:8554/teststream";

    _media_2 = new VlcMedia(url_2, _instance_2);
    //_player->open(_media_2);
    //_player_2->open(_media_2);

    connect(_player_2, &VlcMediaPlayer::end, this, &MainWindow::reconnect);
    connect(_player_2, &VlcMediaPlayer::error, this, &MainWindow::reconnect);

  /*  dgSecondVideo = new dgVLC(ui->widgetVideo_2); //dgSecondVideo = new dgVLC(ui->btWindowMode);
    dgSecondVideo->show();
    dgSecondVideo->show_video(media2);*/

    //запуск видеопотока Kondrat
    videoThr = new VideoThread( ConstInfo::PORT_VIDEO,this,_instance);
    connect(videoThr, SIGNAL(errorConnection(int)), this, SLOT(errorConnection(int)));
    connect(videoThr, SIGNAL(isConnected(int)), this, SLOT(isConnected(int)));
    connect(videoThr, SIGNAL(videoStart( VlcMedia  *)), this, SLOT(startVideoStream( VlcMedia  *)));
    connect(videoThr, SIGNAL(videoStart2( VlcMedia  *)), this, SLOT(receiveMedia2( VlcMedia  *)));
    //videoThr->setParent(this);
    //connect(videoThr, SIGNAL(videoStop()), this, SLOT(stopVideo()));   //11

    videoThr->start();

    // получение уровня заряда батареи планшета
    battLevelThr = new TabletBatteryLevelRequest();
    connect(battLevelThr, SIGNAL(batteryInfo(int)), this, SLOT(setBatteryLevel(int)));

    battLevelThr->start();
    on_btWindowMode_clicked();

//    m_pTcpSocket = new QTcpSocket(this);

//        m_pTcpSocket->connectToHost(ConstInfo::IP_ADDRESS, ConstInfo::PORT_SEND_MSG_AUDIO);
//        connect(m_pTcpSocket, SIGNAL(connected()), this,  SLOT(slotConnected()));
//        connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
//        connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//                    this,     SLOT(slotError(QAbstractSocket::SocketError)));

        //connect(m_ptxtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
        connect(tmr, SIGNAL(timeout()), this, SLOT(slotSendToServer()));
      //  connect(m_pTcpSocket, SIGNAL(slotReadyRead()), this, SLOT(slotSendToServer()));

    //m_pTcpSocket = new QTcpSocket(this);

 /*   m_pTcpSocket->connectToHost(ConstInfo::PORT_SEND_MSG_AUDIO", 5212);
    connect(m_pTcpSocket, SIGNAL(connected()), this,  SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,     SLOT(slotError(QAbstractSocket::SocketError)));

    //connect(m_ptxtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    connect(tmr, SIGNAL(timeout()), this, SLOT(slotSendToServer()));
    connect(m_pTcpSocket, SIGNAL(slotReadyRead()), this, SLOT(slotSendToServer()));*/
  //  on_btWindowMode_clicked();

   /* my_socket->connectToHost(ConstInfo::PORT_SEND_MSG_AUDIO, 5213);

    connect(my_socket, SIGNAL(disconnected()), my_socket, SLOT(deleteLater()));
    connect(my_socket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));*/



        this->serialPort = new QSerialPort(this);
        serialPort->setPortName("COM6");
        serialPort->open(QSerialPort::OpenModeFlag::ReadWrite);
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setParity(QSerialPort::Parity::NoParity);
        serialPort->setDataBits(QSerialPort::DataBits::Data8);
        serialPort->setStopBits(QSerialPort::StopBits::OneStop);

        m_pTcpSocket = new QTcpSocket(this);
        m_pTcpSocket->connectToHost("169.254.8.145", 5210);

        connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
       connect(serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        connect(this, SIGNAL(send(QByteArray)), this, SLOT(sendData(QByteArray)));

        //serialPort->open(QSerialPort::OpenModeFlag::ReadOnly);

        //connect(serialPort, static_cast<void (QSerialPort::*) (QSerialPort::SerialPortError)> (&QSerialPort::error), this, & MainWindow::reconnect_port);
       // connect(serialPort,&QSerialPort::readyRead,this,&MainWindow::onReadyRead);
        //connect(serialPort, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(errorSerial(QSerialPort::SerialPortError)));
        //connect(serialPort,&QSerialPort::error,this, SLOT(reconnect_port()));

}

MainWindow::~MainWindow()
{
    if (dgSecondVideo != NULL){
        delete dgSecondVideo;
    }
    cntrlMsgSendThread ->terminate();

    /*if(serialPort != nullptr){
        serialPort->close();
        delete serialPort;
    }*/
    serialPort->close();

    if(tmr != nullptr){
        delete tmr;
    }

    if(tmr_2 != nullptr){
        delete tmr_2;
    }

    if(sendTimer != nullptr){
        delete sendTimer;
    }

    delete contrState;
    delete robotState;
    pingmodel->finished();
    delete pingmodel;
    robotState = NULL;
    delete showMessage;

    delete _player;
    delete _instance;
    delete _player_2;
    delete _instance_2;
    m_process->close();
    delete m_process;

    delete ui;
}

void MainWindow::onReadyRead()
{
    //serialPort->waitForReadyRead(35);
    QByteArray data;
    int numbersOfBytes = serialPort->bytesAvailable();
    qDebug() << "Bytes size " << QString::number(numbersOfBytes);
    data.clear();
    data = serialPort->readAll();
    qDebug() << "Recieve: " << data;
    emit send(data);
    data.clear();
    receiveDataBuffer.clear();

    /*if(receiveDataBuffer.size() == 20)  {
        qDebug() << "size : " << receiveDataBuffer.size();
        qDebug() << receiveDataBuffer.toHex().toUpper();
    }*/

    if(receiveDataBuffer.size() == 20){

        //counter++;
        //qDebug()<<  counter << " - Buff : "<< receiveDataBuffer.toHex() << "size : " << receiveDataBuffer.size();
        //if(qbaForMrk.size() == 0)
            qbaForMrk.clear();
            qbaForMrk.append(receiveDataBuffer);
            qbaForMrk[12] = (char)0xcc;

            //QByteArray qba_;
            //qba_.append(qbaForMrk[7]);
            /*int xx1, yy1, zz1, xx2, yy2, zz2, ses, hi_U, lo_U, hi_I, lo_I, varValue;
            unsigned res;
            res = (uchar)receiveDataBuffer[2];
            xx1 = (int) res;
            res = (uchar)receiveDataBuffer[3];
            yy1 = (int) res;
            res = (uchar)receiveDataBuffer[4];
            zz1 = (int) res;
            res = (uchar)receiveDataBuffer[5];
            xx2 = (int) res;
            res = (uchar)receiveDataBuffer[6];
            yy2 = (int) res;
            res = (uchar)receiveDataBuffer[7];
            zz2 = (int) res;*/

            /*axisChanged(xx1, 1);
            axisChanged(yy1, 2);
            axisChanged(zz1, 3);
            axisChanged(xx2, 4);
            axisChanged(yy2, 5);
            axisChanged(zz2, 6);*/
           /* contrState->setAxisArmForwardBackward(receiveDataBuffer[2]);
            contrState->setAxisArmForwardBackward(receiveDataBuffer[3]);
            contrState->setAxisArmForwardBackward(receiveDataBuffer[4]);
            contrState->setAxisArmForwardBackward(receiveDataBuffer[5]);
            contrState->setAxisArmForwardBackward(receiveDataBuffer[6]);
            contrState->setAxisArmForwardBackward(receiveDataBuffer[7]);*/
    }
    if(receiveDataBuffer.size() > 20 && receiveDataBuffer[21] == (char)0xcc)
    {
        QByteArray arrayForU2;
        arrayForU2.append(receiveDataBuffer[23]);
        arrayForU2.append(receiveDataBuffer[24]);
        lvlAKB_2(arrayForU2);
        arrayForU2.clear();

        /*unsigned res;
        int hi_U, lo_U, varValue;
        res = (uchar)receiveDataBuffer[23];
        hi_U = (int) res;
        res = (uchar)receiveDataBuffer[24];
        lo_U = (int) res;
        double d_U_ = (hi_U * 256 + lo_U) * 3.3 * 7.8/4095;
        if(d_U_ > 14.5){
            //double d_U = (d_U_ - 15) *100 / 5.5;
            double d_U = (d_U_ - 14.5) *10000 / (6*90) + 2;
            varValue = (int) d_U ;
            //qDebug() << "U : " << varValue;
        }
        if(varValue > 100)  varValue = 100;
        ui->progressBar_pl1->setValue(varValue);*/

        //QTime ct = QTime::currentTime();
        qDebug() << "SES : " << receiveDataBuffer.toHex().toUpper();// << receiveDataBuffer.size() << " time : " << ct.toString();

        /*if(varValue != 0){
            qba_U_[i_U] = varValue;
            //qba_U_[i_U] = QByteArray::number(varValue, 16).toUpper();
            i_U++;
            if(i_U == 49){
                int sum = 0;
                for(int j = 0; j<50; j++){
                    //sum += qba_U_[j];
                    res =(uchar)qba_U_[j];
                    sum += res;
                    //sum += qba_U_[j];
                    qDebug() << "U : " << res << " sum : " << sum << "u[" << j << "] : " << varValue;
                }
                varValue = (int) sum/50;
                i_U = 0;
                //emit changeBatteryLevelRob(varValue);
                //ui->progressBar_pl1->setValue(80);
                qDebug() << "U : " << varValue;
                if(varValue > 100)  varValue = 100;
                ui->progressBar_pl1->setValue(varValue);
                //i_U_rob = varValue;
                qba_U_.clear();
            }

           // qDebug() << "U from MRK send: " << bbb.toHex() << "my d_U : " << d_U << "varValue : " << varValue;
        }*/

        /*int xx1, yy1, zz1, xx2, yy2, zz2;
        res = (uchar)receiveDataBuffer[2];
        xx1 = (int) res;
        res = (uchar)receiveDataBuffer[3];
        yy1 = (int) res;
        res = (uchar)receiveDataBuffer[4];
        zz1 = (int) res;
        res = (uchar)receiveDataBuffer[5];
        xx2 = (int) res;
        res = (uchar)receiveDataBuffer[6];
        yy2 = (int) res;
        res = (uchar)receiveDataBuffer[7];
        zz2 = (int) res;*/

        /*res = (uchar)receiveDataBuffer[1];
        ses = (int) res;*/

        /*contrState->setAxisArmForwardBackward(receiveDataBuffer[2]);
        contrState->setAxisArmForwardBackward(receiveDataBuffer[3]);
        contrState->setAxisArmForwardBackward(receiveDataBuffer[4]);
        contrState->setAxisArmForwardBackward(receiveDataBuffer[5]);
        contrState->setAxisArmForwardBackward(receiveDataBuffer[6]);
        contrState->setAxisArmForwardBackward(receiveDataBuffer[7]);*/
    }
    receiveDataBuffer.clear();
}

/*virtual*//* void MainWindow::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
           );
    connect(pClientSocket, SIGNAL(readyRead()),
            this,          SLOT(slotReadClient())
           );

    sendToClient(pClientSocket, "Server Connected!!!");
}



void MainWindow::slotReadClient()
{
    QByteArray ba;
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    ba = pClientSocket->readAll();
    in.setVersion(QDataStream::Qt_5_6);
    //for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                //break;
            }
            in >> m_nNextBlockSize;
        }

        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {
           // break;
        }
        QTime time = QTime::currentTime();
        QString str = time.toString();

        QString strMessage =
            time.toString();// + " " + "Client has sended - " + str;
        m_ptxt->append(strMessage);

        qDebug() << "strMessage : " << strMessage;
        qDebug() << "in : " << m_nNextBlockSize;
        qDebug() << "ba : " << ba;

        m_nNextBlockSize = 0;

        //sendToClient(pClientSocket, "Server Response: Received \"" + str + "\"");
       // sendToClient(pClientSocket, qbArray_audio);
       // qbArray_audio.clear();

   // }

    QString str_my = " time on the server : "; //QString::number(counter) + " time on the server : "
    QString str_my_2 = "\n";
    //qDebug() << "counter  = " << str;
    QByteArray array;
    array.append(str_my);
    array.append(strMessage);
    array.append(str_my_2);

    //pSocket->write(arrBlock);
   // pClientSocket->write(qbArray_audio);
    m_ptxt->setText(strMessage);

    sendToClient(pClientSocket, qbArray_audio);
    qbArray_audio.clear();
}

void MainWindow::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_6);
   // out << quint16(0) << QTime::currentTime() << qbArray_audio;
    out << quint16(0) << qbArray_audio;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    QString str_my;// = QString::number(counter) + " ";

    QByteArray array;
    array.append(arrBlock);
    qDebug() << " out : " << array.toHex() << "size : " << array.size();
    //array.append(qbArray_audio);

    pSocket->write(arrBlock);
    //pSocket->write(array);
    qbArray_audio.clear();
    //pSocket->write(qbArray_audio);
}*/

quint32 MainWindow::listFolder(QString path){
    QDir currentFolder( path );

    quint32 totalsize = 0;

    currentFolder.setFilter( QDir::Dirs | QDir::Files | QDir::NoSymLinks );
    currentFolder.setSorting( QDir::Name );

    QFileInfoList folderitems( currentFolder.entryInfoList() );

    foreach ( QFileInfo i, folderitems ) {
        QString iname( i.fileName() );
        if ( iname == "." || iname == ".." || iname.isEmpty() )
            continue;

        if ( i.isDir() )
            totalsize += listFolder( path+"/"+iname );
        else
            totalsize += i.size();
    }
    totalsize = totalsize / 1000000;
    return totalsize;
}

void MainWindow::slotReadyRead()
{
    /*QByteArray array;
    qba_From_Mrk.clear();
    while(m_pTcpSocket->bytesAvailable()>0)
    {
        array = m_pTcpSocket->readAll();
    }
    if(array.size() != 0){
        qba_From_Mrk.append(array);
        qDebug() << "Answer : " << array.toHex().toUpper() << " size : " << array.size();
    }

    if(qba_From_Mrk[1] != (char)0x00){
        QByteArray arrayForU;
        arrayForU.append(qba_From_Mrk[0]);
        arrayForU.append(qba_From_Mrk[1]);
        qDebug() << "Array for lvlAKB : " << arrayForU.toHex().toUpper();
        lvlAKB(arrayForU);
        array.clear();
        arrayForU.clear();
    }

    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_5_6); */


        QByteArray Data = m_pTcpSocket->readAll();
        qDebug() << "Data from PM:" << Data.toHex();
        QByteArray array1;
        QByteArray array2;
        array1.append(Data[0]);
        array2.append(Data[1]);
        int meter = convertToDecimal(array1);
        int centimeters = convertToDecimal(array2);

        if(meter < 1) ui->label_8->setStyleSheet("QLabel {background-color:red; color:black}");
        else ui->label_8->setStyleSheet("QLabel{color:black}");
        ui->label_8->setText("В катушке осталось: \n" + QString::number(meter) + " м " + QString::number(centimeters) + " см.");
//        showMessage2->addMessage("В катушке осталось: " + QString::number(meter) + " м " + QString::number(centimeters) + " см.", ConstInfo::TypeMessage::Info);

//        QString str = (QString) Data;
//        qDebug() << Data;
//        QByteArray firstByte;
//        QByteArray secondByte;
//        firstByte.append(Data[0]);
//        secondByte.append(Data[1]);
//        QStringListModel model;
//        int mResult = fromHexToDecimal(firstByte);
//        int smResult = fromHexToDecimal(secondByte);
//        if(mResult <= 1){

//            ui->labelEncoder->setStyleSheet("QLabel { background-color : red; color : blue; }");


//        }
//        else ui->labelEncoder->setStyleSheet("QLabel { color : black; }");
//        QStringList list;
//        QString str2;
//        str2 = "Осталось кабеля: " + QString::number(mResult) + " м, " + QString::number(smResult) + " см";
//        list.append(str2);
//        model.setStringList(list);
//        ui->lvOtherInfo->setModel(model);
//        ui->labelEncoder->setText("Длина кабеля, оставшегося в катушке: " + QString::number(mResult) + " м, " + QString::number(smResult) + " см");



    /*QTime time = QTime::currentTime();
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }
        in >> time;
        //qDebug() << "Socket 5212 work : " << time.toString();

        //m_ptxtInfo->append(time.toString() + " " + str);
        m_nNextBlockSize = 0;
    }*/
}

int MainWindow::convertToDecimal(QByteArray array){
    int fromHexToDec = 0;
    QString arr = array.toHex();
    int result = 0;
    qDebug() << "String number: " << arr;
    for(int i = 0; i < arr.size(); i++){
        if(arr[i] == '0') fromHexToDec = 0;
        else if(arr[i] == '1') fromHexToDec = 1;
        else if(arr[i] == '2') fromHexToDec = 2;
        else if(arr[i] == '3') fromHexToDec = 3;
        else if(arr[i] == '4') fromHexToDec = 4;
        else if(arr[i] == '5') fromHexToDec = 5;
        else if(arr[i] == '6') fromHexToDec = 6;
        else if(arr[i] == '7') fromHexToDec = 7;
        else if(arr[i] == '8') fromHexToDec = 8;
        else if(arr[i] == '9') fromHexToDec = 9;
        else if(arr[i] == 'a') fromHexToDec = 10;
        else if(arr[i] == 'b') fromHexToDec = 11;
        else if(arr[i] == 'c') fromHexToDec = 12;
        else if(arr[i] == 'd') fromHexToDec = 13;
        else if(arr[i] == 'e') fromHexToDec = 14;
        else if(arr[i] == 'f') fromHexToDec = 15;
        result += fromHexToDec *pow(16, arr.size() - i - 1);
    }
    qDebug() << "Decimal: " << result;
    return result;
}
int MainWindow::fromHexToDecimal(QByteArray arrayForU2){
    int fromHexToDec = 0;
    int result = 0;
    QByteArray bb;
    QString arr;
    bb.append(arrayForU2[0]);
    bb.append(arrayForU2[1]);
    arr = arrayForU2.toHex();
    qDebug() << arr;
    for(int i = 0; i < arr.size(); i++){
        if(arr[i] == '0') fromHexToDec = 0;
        else if(arr[i] == '1') fromHexToDec = 1;
        else if(arr[i] == '2') fromHexToDec = 2;
        else if(arr[i] == '3') fromHexToDec = 3;
        else if(arr[i] == '4') fromHexToDec = 4;
        else if(arr[i] == '5') fromHexToDec = 5;
        else if(arr[i] == '6') fromHexToDec = 6;
        else if(arr[i] == '7') fromHexToDec = 7;
        else if(arr[i] == '8') fromHexToDec = 8;
        else if(arr[i] == '9') fromHexToDec = 9;
        else if(arr[i] == 'a') fromHexToDec = 10;
        else if(arr[i] == 'b') fromHexToDec = 11;
        else if(arr[i] == 'c') fromHexToDec = 12;
        else if(arr[i] == 'd') fromHexToDec = 13;
        else if(arr[i] == 'e') fromHexToDec = 14;
        else if(arr[i] == 'f') fromHexToDec = 15;

        result += fromHexToDec * pow(16, arr.size() - i - 1);
    }
    qDebug() << "Decimal: " << result;
    return result;
}


void MainWindow::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
        "ERROR: " + (err == QAbstractSocket::HostNotFoundError ?
                     "The host was not found." :
                     err == QAbstractSocket::RemoteHostClosedError ?
                     "The remote host is closed." :
                     err == QAbstractSocket::ConnectionRefusedError ?
                     "The connection was refused." :
                     QString(m_pTcpSocket->errorString())
                    );
    qDebug() << strError;
}

void MainWindow::lvlAKB(QByteArray qbaU){
    QByteArray bb;
    int varValue_2, xx, yy;
    bb.append(qbaU[0]);
    bb.append(qbaU[1]);

    unsigned res;
    res = (uchar)bb[0];
    xx = (int) res;
    res = (uchar)bb[1];
    yy = (int)res;

    double d_U_ = (xx * 256 + yy) * 2.5 * 18.87/1023;
    if(d_U_ > 21){
        double d_U2_ = (d_U_ - 21) *100 / 7;
        varValue_2 = (int) d_U2_;
    }
    qDebug() << "U PM : " << bb.toHex().toUpper() << "varValue : " << varValue_2;
    if(varValue_2 > 100)  varValue_2 = 100;
    ui->progressBar_rbt->setValue(varValue_2);

    /*if(varValue != 0){
        qba_U_MRK[i_U2] = varValue;
        i_U2++;
        if(i_U2 == 9){
            int sum = 0;
            for(int j = 0; j<10; j++){
                sum += qba_U_MRK[j];
            }
            varValue = (int) sum/10;
            i_U2 = 0;
            //emit changeBatteryLevelRob(varValue);
            if(varValue > 100)  varValue = 100;
            ui->progressBar_rbt->setValue(varValue);

            //i_U_rob = varValue;
            qDebug() << "qba_U : " << qba_U_MRK.toHex().toUpper();
            qba_U_MRK.clear();
        }
*/
    //}
    bb.clear();
}

void MainWindow::lvlAKB_2(QByteArray qbaU){
    QByteArray bb;
    int varValue, xx, yy;
    bb.append(qbaU[0]);
    bb.append(qbaU[1]);

    unsigned res;
    res = (uchar)bb[0];
    xx = (int) res;
    res = (uchar)bb[1];
    yy = (int)res;

    //double d_U = (qbaU[0] * 256 + qbaU[1]) * 3.3 * 7.8/4095;
    double d_U = (xx * 256 + yy) * 3.3 * 7.8/4095;
    if(d_U > 14.5){
        double d_U2 = (d_U - 14.5) *10000 / (6*90);
        varValue = (int) d_U2;
        qDebug() << "U for MRK : " << bb.toHex().toUpper() << "varValue : " << varValue;
        if(varValue > 100 )  varValue = 100;
        ui->progressBar_pl1->setValue(varValue);
        //ui->progressBar_pl1->setTextVisible(false);

    }else{
        varValue = 0;
        ui->progressBar_pl1->setValue(varValue);
        //ui->progressBar_pl1->setTextVisible(false);
    }
    bb.clear();
}

QByteArray MainWindow::speed(QByteArray qba_XY){
    QByteArray qba_speed;
    int x, y;
    unsigned res;
    res = (uchar)qba_XY[0];
    x = (int) res;
    res = (uchar)qba_XY[1];
    y = (int)res;

    int d_X = (y * 2 - x + 127)/2;
    int d_Y = (y * 2 + x - 127)/2;

    if(d_X < 0){
        d_X = 0;
    }
    if(d_X > 254){
        d_X = 254;
    }

    if(d_Y < 0){
        d_Y =  0;
    }
    if(d_Y > 254){
        d_Y = 254;
    }

    /*if(d_X < 120 && d_Y < 120){
        qba_speed[0] = d_Y;
        qba_speed[1] = d_X;
    }else{
        qba_speed[0] = d_X;
        qba_speed[1] = d_Y;
    }*/

    qba_speed[0] = d_X;
    qba_speed[1] = d_Y;

    //qDebug() << "x = " << d_X << " y = " << d_Y << " qba_speed = " << qba_speed.toHex().toUpper();

    return qba_speed;
}

void MainWindow::slotSendToServer()
{
    QByteArray  arrBlock;
   // arrBlock.append(qbArray_audio);

    QDataStream out(&arrBlock, QIODevice::ReadWrite);
    //out.setVersion(QDataStream::Qt_5_6);
    //out << quint16(0) << QTime::currentTime();// << qbArray_audio;
    out << quint16(0) << qbArray_audio;
    //out << quint16(0) << arrBlock;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

   /* QByteArray array;
    array.append(arrBlock);
    qDebug() << " out : " << array.toHex() << "size : " << array.size();*/
    //array.append(qbArray_audio);

   /* short* proba = (short*)m_buffer.data();
    qDebug() << "proba : " << proba; << "size : " << array.size();*/

    if(qbaForMrk.size() == 20){
        /*QByteArray qbaForMrk_2;
        qbaForMrk_2.append(qba_cc);
        qbaForMrk_2.append(qbaForMrk);*/
        QByteArray qbaSpeed_in;
        QByteArray qbaSpeed_out;
        if(qbaForMrk.size() == 20){
            qbaSpeed_in.append(qbaForMrk[2]);
            qbaSpeed_in.append(qbaForMrk[3]);
            qbaSpeed_out.append(speed(qbaSpeed_in));

        /*qDebug() << "Speed in : " << qbaSpeed_in.toHex().toUpper() << " : " << qbaSpeed_in.size()
                 << "Speed out : " << qbaSpeed_out.toHex().toUpper() << " : " << qbaSpeed_out.size();
        */
        //qDebug() << "qbaForMrk in : " << qbaForMrk.toHex().toUpper();
        qbaForMrk[2] = qbaSpeed_out[0];
        qbaForMrk[3] = qbaSpeed_out[1];
        //qDebug() << "qbaForMrk out : " << qbaForMrk.toHex().toUpper();

        qbaSpeed_in.clear();
        qbaSpeed_out.clear();
        }

        if(qbaForMrk[17] == (char)0x01){
            ui->label_8->setStyleSheet("QLabel {background-color : blue; color: yellow;}");
            ui->label_8->setText("Изделие МРК-А1 разработано\n"
                                 "ОАО \"АГАТ - системы управления\" - \n"
                                 "управляющая компания холдинга\n"
                                 "\"Геоинформационные системы управления\"");
                                 //"г. Минск, 2020 г.");
        }

        if(qbaForMrk[17] == (char)0x00){
            ui->label_8->setStyleSheet("QLabel {background-color : black; color: black;}");
            //ui->label_8->setText("");
        }

        if(qbaForMrk[17] == (char)0x02){
            on_btConnection_clicked();
        }

        if(qbaForMrk[10] == (char)0x01){
            on_btPhoto_clicked();
        }

        //if(qbaForMrk[12] == (char)0xff && (((qbaForMrk[15] >> 2) & 1u) == 1 || ((qbaForMrk[15] >> 3) & 1u) == 1) ){
        /*if(qbaForMrk[12] == (char)0xff){
            ui->infoLabel->setText(trUtf8("РОБОТ БУДЕТ СЛОЖЕН В ПОХОДНОЕ ПОЛОЖЕНИЕ! КНОПКИ НЕ НАЖИМАТЬ!"));
            tmr_5s->start(5000);
        }*/

        /*if( qbaForMrk[12] == (char)0xff && (((qbaForMrk[16] >> 0) & 1u) == 1 || ((qbaForMrk[16] >> 1) & 1u) == 1) ){
            ui->infoLabel->setText(trUtf8("НЕОБХОДИМО ПЕРЕЙТИ В РЕЖИМ УПРАВЛЕНИЯ МАНИПУЛЯТОРОМ!"));
        }*/

        if( ((qbaForMrk[10] >> 1) & 1u) == 1 && iRecVideo == 0){
            iRecVideo = 1;
            on_btCamera_2_clicked();
            ui->infoLabel->setText(trUtf8("ИДЕТ ЗАПИСЬ ВИДЕО!"));
            tmr_5s->start(5000);
        }

        if( ((qbaForMrk[10] >> 1) & 1u) == 0 && iRecVideo == 1){
            iRecVideo = 0;
            on_btCamera_2_clicked();
            ui->infoLabel->setText("ЗАПИСЬ ВИДЕО ОСТАНОВЛЕНА!");
            tmr_5s->start(5000);
        }

        if(qbaForMrk[11] == (char)0x02){
            on_btWindowMode_clicked();
        }

        if(qbaForMrk[11] == (char)0x01){
            on_btWindowMode_clicked();
            QProcess::startDetached("shutdown -s -f -t 00");
        }

        if( ((qbaForMrk[15] >> 2) & 1u) == 1 || ((qbaForMrk[15] >> 3) & 1u) == 1){
            if(iPlatf_Monip == 0 || iPlatf_Monip == 1){
                ui->infoLabel->setText(trUtf8("ВКЛ. РЕЖИМ УПРАВЛЕНИЯ МАНИПУЛЯТОРОМ!"));
                iPlatf_Monip = 2;
            }
        }

        if( ((qbaForMrk[16] >> 0) & 1u) == 1 || ((qbaForMrk[16] >> 1) & 1u) == 1){
            if(iPlatf_Monip == 0 || iPlatf_Monip == 2){
                ui->infoLabel->setText(trUtf8("ВКЛ. РЕЖИМ УПРАВЛЕНИЯ ПЛАТФОРМОЙ МОБИЛЬНОЙ!"));
                iPlatf_Monip = 1;
            }
            //ui->statusBar->showMessage("ВКЛ. РЕЖИМ УПРАВЛЕНИЯ ПЛАТФОРМОЙ!", 5000);
        }

        if(qbaForMrk[15] == (char)0x00 && qbaForMrk[16] == (char)0x00){
            iPlatf_Monip = 0;
        }

        //qDebug() << "IRECVIDEO : " << iRecVideo << "IPLAT_MON : " << iPlatf_Monip;

        if( ((qbaForMrk[16] >> 0) & 1u) == 0 && ((qbaForMrk[16] >> 1) & 1u) == 0 &&
            ((qbaForMrk[15] >> 2) & 1u) == 0 && ((qbaForMrk[15] >> 3) & 1u) == 0 ){
            ui->infoLabel->clear();
        }

        /*if( ((qbaForMrk[8] >> 0) & 1u) == 0 && ((qbaForMrk[8] >> 1) & 1u) == 0 &&
            ((qbaForMrk[8] >> 2) & 1u) == 0 && ((qbaForMrk[8] >> 3) & 1u) == 0 ){
            ui->infoLabel->setText(trUtf8("НЕ ВЫБРАНА ЛЕВАЯ КАМЕРА!"));
            //ui->widgetVideo->setVisible(false);
            //_player->stop();
        }else{
            ui->widgetVideo_2->setVisible(true);
        }*/

        /*if( ((qbaForMrk[8] >> 4) & 1u) == 0 && ((qbaForMrk[8] >> 5) & 1u) == 0 &&
            ((qbaForMrk[8] >> 6) & 1u) == 0 && ((qbaForMrk[8] >> 7) & 1u) == 0 ){
            ui->infoLabel->setText(trUtf8("НЕ ВЫБРАНА ПРАВАЯ КАМЕРА!"));*/
            //ui->widgetVideo_2->setVisible(false);
            //_player_2->stop();
        //}
        /*else{
            ui->widgetVideo_2->setVisible(true);
            //_player_2->open(_media_2);
        }*/

        /*if(qbaForMrk[8] == (char)0x00){
            ui->infoLabel->setText(trUtf8("НЕ ВЫБРАНЫ КАМЕРЫ!"));
            /*ui->widgetVideo->setVisible(false);
            ui->widgetVideo_2->setVisible(false);*/
        //}
        /*else{
            ui->widgetVideo->setVisible(true);
            ui->widgetVideo_2->setVisible(true);
        }*/

        /*if(qbaForMrk[17] == 1){
            on_btWindowMode_clicked();
            QProcess::startDetached("restart-computer -s -f -t 00");
        }*/

        qbaForMrk[19] = (char)0xaa;
        qbaForMrk.prepend(qba_cc);

       /* QByteArray buff_2;
        buff_2.append(receiveDataBuffer[3]);
        buff_2.append(receiveDataBuffer[4]);
        buff_2.append(receiveDataBuffer[5]);
        buff_2.append(receiveDataBuffer[6]);
        buff_2.append(receiveDataBuffer[7]);
        buff_2.append(receiveDataBuffer[8]);
        qDebug() << buff_2.toHex().toUpper() << " : " << qbaForMrk.size();
        buff_2.clear();*/

    }

    if(qbaForMrk.size() == 21){
    //m_pTcpSocket->write(qba_cc); //m_pTcpSocket->write(arrBlock);
    m_pTcpSocket->write(qbaForMrk);
 //   qDebug() << " out : " << arrBlock.trimmed() << "size : " << arrBlock.size();
    //int counter_ = counter;
        qDebug() << qbaForMrk.toHex().toUpper() << " size = " << qbaForMrk.size();
        /*int x;
        /*unsigned res;
        QString str;
        res = (uchar)qbaForMrk[3];
        x = (int) res;
        str = str + "Speed 1 = " + QString::number(x);
        res = (uchar)qbaForMrk[4];
        x = (int) res;
        str = str + "   Speed 2 = " + QString::number(x);
        res = (uchar)qbaForMrk[5];
        x = (int) res;
        str = str + "   Speed 3 = " + QString::number(x);
        res = (uchar)qbaForMrk[6];
        x = (int) res;
        str = str + "   Speed 4 = " + QString::number(x);
        res = (uchar)qbaForMrk[7];
        x = (int) res;
        str = str + "   Speed 5 = " + QString::number(x);
        res = (uchar)qbaForMrk[8];
        x = (int) res;
        str = str + "   Speed 6 = " + QString::number(x);
        ui->infoLabel->setText(str);*/
    }
    qbaForMrk.clear();
    //qbaForMrk_2.clear();
    //}
    qbArray_audio.clear();
}

void MainWindow::slotConnected()
{
    qDebug() << "CONNECTED SIGNAL!!!";
}

/*void MainWindow::audio_for_tcp(QTcpSocket* pSocket, QByteArray ba_a, int len_ba_a)
{
     QTcpSocket* pClientSocket_2 = (QTcpSocket*)sender();
     qDebug() << "audio : " << ba_a.toHex() << "size : " << len_ba_a;
     my_socket->write(ba_a);
     qbArray_audio.clear();

}*/

void MainWindow::updateTime_my_2()
{
    ++counter_my_2;
    //ui->progressBar_rbt->setValue(75);

    //qDebug() << "Timer 15 min № : " << counter_my_2;

    if(counter_btvideo_click % 2 != 0){
        if(counter_btvideo_click < 11){
            switch (counter_my_2) {
            case 1:
                m_process->close();
                //name_file_videos = name_file_videos.replace(name_file_videos.size() - 4, 8, "_p01.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p02.avi");
                break;
            case 2:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p02.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p03.avi");
                break;
            case 3:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p03.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p04.avi");
                break;
            case 4:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p04.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p05.avi");
                break;
            case 5:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p05.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p06.avi");
                break;
            case 6:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p06.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p07.avi");
                break;
            case 7:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p07.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p08.avi");
                break;
            case 8:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p08.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p09.avi");
                break;
            case 9:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p09.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p10.avi");
                break;
            case 10:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p10.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p11.avi");
                break;
            case 11:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p11.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p12.avi");
                break;
            case 12:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p12.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p13.avi");
                break;
            case 13:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p13.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p14.avi");
                break;
            case 14:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p14.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p15.avi");
                break;
            case 15:
                m_process->close();
                //name_file_videos = name_file_videos.replace(24, 8, "_p15.avi");
                m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                 " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                 " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p16.avi");
                break;
            case 16:
                m_process->close();
                on_btCamera_2_clicked();
                break;
            default:
                break;
            }

        }
    }
}

void MainWindow::update_progressBar()
{
    ui->infoLabel->clear();
    tmr_5s->stop();
}
//Initialize audio

/*QAudioDeviceInfo MainWindow::getAudioDevice(QString name) {
    QAudioDeviceInfo device;
    QList<QAudioDeviceInfo> devices =
        QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for(int i = 0; i < devices.size(); ++i) {
        if(devices.at(i).deviceName() == name) {
            device = devices.at(i);
            qDebug() << "name : " << name;
            break;
        }
    }
    return device;
}*/

void MainWindow::initializeAudio()
{
    m_format.setSampleRate(44100); //set frequency to 8000
    m_format.setChannelCount(1); //set channels to mono
    m_format.setSampleSize(16); //set sample sze to 16 bit
    m_format.setSampleType(QAudioFormat::UnSignedInt ); //Sample type as usigned integer sample
    m_format.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    m_format.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QString deviceId = "Динамики (Realtek High Definition Audio)";
   //QString deviceId = "Микрофон (Realtek High Definition Audio)";
    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());

    //QAudioDeviceInfo infoIn = getAudioDevice(deviceId);


    /*QAudioDeviceInfo infoIn = getAudioDevice("default");
    if(myDevice.isNull()) {
        qCritical("Device not found");
    }*/


    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());

    qDebug() << "name output device : " << infoOut.deviceName();//.append(deviceId);
    qDebug() << "format device : " << m_format;

    if (!infoOut.isFormatSupported(m_format))
    {
       //Default format not supported - trying to use nearest
        m_format = infoOut.nearestFormat(m_format);
        qDebug() << "Error audio format!";
    }
    createAudioInput();
    createAudioOutput();

    m_output= m_audioOutput->start();
     //Audio input device
    m_input = m_audioInput->start();
    connect(m_input, SIGNAL(readyRead()), SLOT(readMore()));

    qDebug() << "AUDIO START!!!";
}

void MainWindow::createAudioOutput()
{
    m_audioOutput = new QAudioOutput(m_Outputdevice, m_format, this);
}
void MainWindow::createAudioInput()
{
    if (m_input != 0) {
        disconnect(m_input, 0, this, 0);
        m_input = 0;
        qDebug() << "Error AUDIO input!!!";
    }

    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);
}
void MainWindow::readMore()
{
    qDebug() << "AUDIO read START!!!";
    //Return if audio input is null
    if(!m_audioInput){
        qDebug() << "Error initialization input audio!!!";
        return;
    }

    //Check the number of samples in input buffer
    qint64 len = m_audioInput->bytesReady();

    //Limit sample size
    if(len > 4096)
        len = 4096;
    //Read sound samples from input device to buffer
    qint64 l = m_input->read(m_buffer.data(), len);
   // qDebug() << "m_buffer : " << m_buffer.data();
    if(l > 0)
    {
        //Assign sound samples to short array
        short* resultingData = (short*)m_buffer.data();
        short *outdata=resultingData;
        outdata[ 0 ] = resultingData [ 0 ];

         int iIndex;
       //  if(ui->chkRemoveNoise->checkState() == Qt::Checked)
       //  {
                //Remove noise using Low Pass filter algortm[Simple algorithm used to remove noise]
                for ( iIndex=1; iIndex < len; iIndex++ )
                {
                    outdata[ iIndex ] = 0.333 * resultingData[iIndex ] + ( 1.0 - 0.333 ) * outdata[ iIndex-1 ];
                }
        //  }

         for ( iIndex=0; iIndex < len; iIndex++ )
         {
             //Cange volume to each integer data in a sample
             outdata[ iIndex ] = ApplyVolumeToSample( outdata[ iIndex ]);
         }

         qbArray_audio.append((char*)outdata);

         //qDebug() << "size : " << qbArray_audio.size() << "qbArray_audio : " << qbArray_audio.toHex(); /*.trimmed()*/

       /*  bool ok;
         qDebug() << "qbArray_audio_2 : " << qbArray_audio.toUShort(&ok, 16);
        // qDebug() << "outdata : " << (char*)outdata;*/

       /*bool ok;
       const char* outdata_= src_audio.toLocal8Bit().data();
       qDebug() << "outdata_ : " << outdata;

       short* resultingData = (short*)m_buffer.data();
       short* QString :: number (qbArray_audio, 16)

       QByteArray data_2 = entity->getData();
       QString s_data = QString::fromUtf16((ushort *)qbArray_audio.data());*/

       /*byte data[] = new byte[bufferSize]; // Создание буфера
         while (isRecording) {
           read = recorder.read(data, 0, bufferSize); // Чтение данных с микрофона в буфер
           if (AudioRecord.ERROR_INVALID_OPERATION != read) {
             totalDataSize += data.length;
             sendPacket(PackageType.client_send_data, data); // Отправка буфера на сервер
           }
         }
       closeSocket();*/

       m_output->write((char*)outdata, len);

       m_pTcpSocket->write((char*)outdata, len);
       qbArray_audio.clear();
    }
}

int MainWindow::ApplyVolumeToSample(short iSample)
{
    //Calculate volume, Volume limited to  max 35535 and min -35535
    return std::max(std::min(((iSample * m_iVolume) / 50) ,35535), -35535);

}


/*void MainWindow::on_pushButton_clicked()
{
    //Audio output device
    //m_output= m_audioOutput->start();
     //Audio input device
    //m_input = m_audioInput->start();
    //connect readyRead signal to readMre slot.
    //Call readmore when audio samples fill in inputbuffer
    //connect(m_input, SIGNAL(readyRead()), SLOT(readMore()));
     m_iVolume = 0;
     //disconnect(m_input, 0, this, 0);

}*/

void MainWindow::startVideoStream( VlcMedia  *_media)
{

    qDebug()<< " _player->open ";
    startMedia=true;
    _player->open(_media);


    //_media_2 = new VlcMedia(url_2, _instance_2);
    _player_2->open(_media_2);
}


void MainWindow::joystickInit()
{
    QStringList avaliableJoystickName = VJoystickAdapter::getAvaliableJoystickName();
    //QString str = "avaliableJoystickName.count: " + QString::number(avaliableJoystickName.count());
    //ui->lbTest->setText(str);
    QTest::qWait(3000);
    if (avaliableJoystickName.count() == 0)
    {
      
      //emit message(trUtf8("Джойстик не обнаружен"), ConstInfo::Info);
      //  ui->lbTest->setText("DRGER");
     
    }
    if (avaliableJoystickName.count() > 1)
    {
      emit message(trUtf8("Несколько джойстиков"), ConstInfo::Info);
    }
    if (avaliableJoystickName.count() == 1)
    {
        // инициализация джойстика
        if(joystick->open(0))
        {
            connect(joystick, SIGNAL(sigButtonChanged(int, bool)), this, SLOT(buttonChanged(int,bool)));
            connect(joystick, SIGNAL(sigAxisChanged(int,int)), this, SLOT(axisChanged(int,int)));
        }
    }

}

void MainWindow::buttonChanged(int id, bool state)
{
  /*  if (state == false)
    // отпускаем клавишу
    {
        switch (id) {
        case ConstInfo::BUT_ARM:
        {
            if (!isTwoJoystick){
                contrState->nextArmMode();
            }
            else {
                if (contrState->getArmMode() == ARM_123){
                    contrState->setArmMode(ARM_456);
                }
                else{
                    contrState->setArmMode(ARM_123);
                }
            }
            changesStateButtonArm(); //kun
           // changeLightIndicatorArm(); //kun
            break;
        }
        case ConstInfo::BUT_MOVE:
        {
            contrState->nextMoveMode();
            changesStateButtonMove();  //kun
            //changeLightIndicatorMove(); //kun
            break;
        }
        default:
            break;
        }
    }*/
}
//kun
void MainWindow::changesStateButtonArm()
{
    /*switch (contrState->getArmMode()) {
    case ARM_123:
         on_btArm456_clicked();//on_btArm123_clicked();
         break;
    case ARM_234:
         on_btArm456_clicked();//on_btArm234_clicked();
         break;
    case ARM_456:
         on_btArm456_clicked();
         break;
    default:
        break;
    }*/
}
void MainWindow::changesStateButtonMove()
{
    switch (contrState->getMoveMode()) {
    case MOVE_STOP:
         on_btStopMove_clicked();
         break;
    case MOVE_SLOW:
        on_btSlow_clicked();
        break;
    case MOVE_FAST:
       on_btFast_clicked();
        break;
    default:
        break;
    }
}
//  kun

void MainWindow::axisChanged(int id, int state)
{
    //emit message("id axis = " + QString::number(id) + ", state = " + QString::number(state), ConstInfo::Info);
    if (!isTwoJoystick){
        if (isStop && ((id == ConstInfo::AXIS_FORWARD_BACKWARD) or (id == ConstInfo::AXIS_LEFT_RIGHT) or (id == ConstInfo::AXIS_ROTATION))) return;
        if ((modeMove == MOVE_FAST) && ((id == ConstInfo::AXIS_ARM_FORWARD_BACKWARD) or (id == ConstInfo::AXIS_ARM_LEFT_RIGHT) or (id == ConstInfo::AXIS_ARM_ROTATION))) return;
    }

    QString id_speed;
    QString id_speed_2;

    int ind_;
    ind_ = qba_cc[0];
    switch (id) {
    if(ind_ == 204){
    case ConstInfo::AXIS_FORWARD_BACKWARD:
    {
        contrState->setAxisForwardBackward(FuncUtils::JSVal2RobotVal(state, contrState->getButSpeedVal()));
        id_speed = QString::number(id);
        id_speed_2 = QString::number(state);
        str_res_1 = "id : " + id_speed + "speed : " + id_speed_2;
        //qDebug() << "id : " << id_speed << "speed : " << id_speed_2;
        break;
    }
    case ConstInfo::AXIS_LEFT_RIGHT:
    {
        contrState->setAxisLeftRight(FuncUtils::JSVal2RobotVal(state, contrState->getButSpeedVal()));
        id_speed = QString::number(id);
        id_speed_2 = QString::number(state);
        str_res_2 = "id : " + id_speed + "speed : " + id_speed_2;
        //qDebug() << "id : " << id_speed << "speed : " << id_speed_2;
        break;
    }
    case ConstInfo::AXIS_ROTATION:
    {
        contrState->setAxisRotation(FuncUtils::JSVal2RobotVal(state, contrState->getButSpeedVal()));
        id_speed = QString::number(id);
        id_speed_2 = QString::number(state);
        str_res_3 = "id : " + id_speed + "speed : " + id_speed_2;
        //qDebug() << "id : " << id_speed << "speed : " << id_speed_2;
        break;
    }
    }
    case ConstInfo::AXIS_ARM_FORWARD_BACKWARD:
    {
        contrState->setAxisArmForwardBackward(FuncUtils::JSVal2RobotVal(state, ConstInfo::FAST_MODE));
        id_speed = QString::number(id);
        id_speed_2 = QString::number(state);
        str_res_4 = "id : " + id_speed + "speed : " + id_speed_2;
        //qDebug() << "id : " << id_speed << "speed : " << id_speed_2;
        break;
    }
    case ConstInfo::AXIS_ARM_LEFT_RIGHT:
    {
        contrState->setAxisArmLeftRight(FuncUtils::JSVal2RobotVal(state, ConstInfo::FAST_MODE));
        id_speed = QString::number(id);
        id_speed_2 = QString::number(state);
        str_res_5 = "id : " + id_speed + "speed : " + id_speed_2;
        //qDebug() << "id : " << id_speed << "speed : " << id_speed_2;
        break;
    }
    case ConstInfo::AXIS_ARM_ROTATION:
    {
        contrState->setAxisArmRotation(FuncUtils::JSVal2RobotVal(state, ConstInfo::FAST_MODE));
        id_speed = QString::number(id);
        id_speed_2 = QString::number(state);
        str_res_6 = "id : " + id_speed + "speed : " + id_speed_2;
        //qDebug() << "id : " << id_speed << "speed : " << id_speed_2;
        break;
    }
    default:
        break;
    }

    QString str;
    /*str= "AXIS_FORWARD_BACKWARD: " + QString::number(contrState->getAxisForwardBackward()) +"\n"+
         "AXIS_LEFT_RIGHT: " + QString::number(contrState->getAxisLeftRight()) +"\n"+
         "AXIS_ROTATION: " + QString::number(contrState->getAxisRotation()) +"\n"+
         "AXIS_ARM_FORWARD_BACKWARD: " + QString::number(contrState->getAxisArmForwardBackward()) +"\n"+
         "AXIS_ARM_LEFT_RIGHT: " + QString::number(contrState->getAxisArmLeftRight()) +"\n"+
         "AXIS_ARM_ROTATION: " + QString::number(contrState->getAxisArmRotation());*/

    str= "F_B: " + QString::number(contrState->getAxisForwardBackward()) +
         "L_R: " + QString::number(contrState->getAxisLeftRight()) +
         "ROT: " + QString::number(contrState->getAxisRotation()) +
         "F_B_2: " + QString::number(contrState->getAxisArmForwardBackward()) +
         "L_R_2: " + QString::number(contrState->getAxisArmLeftRight()) +
         "ROT_2: " + QString::number(contrState->getAxisArmRotation()) + "   " +
            str_res_1 + str_res_2 + str_res_3 + str_res_4 + str_res_5 + str_res_6;

    //ui->infoLabel->setText(str);

    if (cntrlMsgSendThread != NULL)
        cntrlMsgSendThread->recalcTimePeriod();
}

void MainWindow::on_btVideo_clicked()
{
    switch( QMessageBox::question(
                this,
                "Сложить манипулятор",
                "Перед сложением манипулятора СЛОЖИТЕ ФЛИППЕРЫ!\n"
                "Будут выключены функции управления робота!",
                QMessageBox::Yes | QMessageBox::No) )
    {
      case QMessageBox::Yes:
        qba_cc[0] = (char) 0xbb;
        //on_btStopMove_clicked();
        ui->btAudio->setEnabled(false);
        ui->btVideo->setEnabled(false);
        ui->btConnection->setEnabled(true);
        ui->btStopMove->setEnabled(false);
        ui->btFast->setEnabled(false);
        ui->btSlow->setEnabled(false);
        //ui->btCamera->setEnabled(false);
        //ui->btCamera_2->setEnabled(false);
        ui->btLight->setEnabled(false);
        ui->btLightIR->setEnabled(false);
        break;
      case QMessageBox::No:
        break;
    /*  case QMessageBox::Cancel:
        break;*/
      default:
        break;
    }
    //qDebug() << "qba_cc : " << qba_cc.toHex();

   // if (ui->infoLabel->text()==" ") {
    /*if (ui->btCamera_2->text() == "ВИДЕО"){
         ui->infoLabel->setText(trUtf8("ВИДЕО ЗАПИСЬ!"));
        //ui->infoLabel lbRecordFile->setText(trUtf8("ЗАПИСЬ ВИДЕО!"));
         //ui->btVideo->setText(trUtf8("ВИДЕО\nСТОП"));
         ui->btCamera_2->setText(trUtf8("ВИДЕО СТОП"));

        // m_process = new QProcess(this);

         QDir dir(ConstInfo::DIR_VIDEOS);

         if (!dir.exists())
             QDir().mkdir(ConstInfo::DIR_VIDEOS);
         //QString name_file_videos=ConstInfo::DIR_VIDEOS+"/video-record.avi"; //+QDateTime::currentDateTime().toString("dd-MM-yy_hh-mm-ss")+".avi";

         m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                          " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                          " -r 15 -preset ultrafast -tune zerolatency -crf 50 -pix_fmt yuv420p " + name_file_videos + "_p01.avi");
         qDebug() << "GO VIDEO RECORD!";

        /* m_process->start("ffmpeg -i ""rtsp://"
                          +ConstInfo::IP_ADDRESS+":"
                           +QString::number(ConstInfo::PORT_VIDEO)+ConstInfo::PATH_VIDEO
                           +""" -vcodec copy -acodec copy -y "+name_file_videos);*/

   /* }
    else {
        ui->infoLabel->setText(" ");
        ui->btCamera_2->setText(trUtf8("ВИДЕО ЗАПИСЬ"));
        //ui->btVideo->setText(trUtf8("ВИДЕО\nЗАПИСЬ"));
       /* m_process->close();
        delete m_process;*/
   /* }

     if (!startMedia)
     {
         QMessageBox::warning(
             this,
             "Предупреждение",
             "Видео еще не запущено" );
      }
   /* else
    {
       onRecordVideo();

      dgVideo *dg = new dgVideo(ui->btCamera_2,btNameRecord);
       //dgVideo *dg = new dgVideo(ui->btVideo,btNameRecord);
       connect(dg, SIGNAL(recordVideo()), this, SLOT(onRecordVideo()));
       dg->exec();
    }*/

}

void MainWindow::onRecordVideo()
{
    if (ui->infoLabel->text()==" ")
       {
          ui->infoLabel->setText(trUtf8("Запись видео ! "));
        // btNameRecord="Остановить запись";

         QMetaObject::invokeMethod(videoThr, "recordFileVideo", Qt::QueuedConnection,//Qt::DirectConnection,
                                     Q_ARG(bool, true));
          //videoThr->recordFileVideo(true);
       }
    else
       {

          ui->infoLabel->setText(" ");
         // btNameRecord="Видеозапись";
          QMetaObject::invokeMethod(videoThr, "recordFileVideo", Qt::QueuedConnection,//Qt::DirectConnection,
                                     Q_ARG(bool, false));
          //videoThr->recordFileVideo(false);
       }
}

void MainWindow::on_btAudio_clicked()
{
    dgAudio *dg = new dgAudio(ui->btAudio);
    connect(dg, SIGNAL(Talk()), this, SLOT(onTalk()));
    connect(dg, SIGNAL(Listen()), this, SLOT(onListen()));
    dg->setGeometry(1025, 100, 220, 169);
    dg->exec();
}

void MainWindow::on_btLight_clicked()
{
    DgLight *dg = new DgLight(ui->btLight, false);
    connect(dg, SIGNAL(lightOff(bool)), this, SLOT(lightOff(bool)));
    connect(dg, SIGNAL(setLight(int,int,bool)), this, SLOT(setLight(int,int,bool)));
    dg->setGeometry(1045, 100, 220, 169);
    dg->exec();
}

void MainWindow::on_btLightIR_clicked()
{
    DgLight *dg = new DgLight(ui->btLightIR, true);
    connect(dg, SIGNAL(lightOff(bool)), this, SLOT(lightOff(bool)));
    connect(dg, SIGNAL(setLight(int,int,bool)), this, SLOT(setLight(int,int,bool)));
    dg->setGeometry(1045, 100, 220, 169);
    dg->exec();
}


void MainWindow::on_btRemote_clicked()
{
    emit message(trUtf8("Нажата кнопка Удаленно"), ConstInfo::Warning);
}

void MainWindow::clearColorFilterArm()
{
    ui->btArm123->setStyleSheet(ConstInfo::NO_COLOR);
    ui->btArm234->setStyleSheet(ConstInfo::NO_COLOR);
    ui->btArm456->setStyleSheet(ConstInfo::NO_COLOR);
}
void MainWindow::clearColorFilterArmR()
{

    ui->btRArm123->setStyleSheet(ConstInfo::NO_COLOR);
//    ui->btRArm156->setStyleSheet(ConstInfo::NO_COLOR);
    ui->btRArm456->setStyleSheet(ConstInfo::NO_COLOR);

}

void MainWindow::clearColorFilterMove()
{
    ui->btStopMove->setStyleSheet(ConstInfo::NO_COLOR);
    ui->btSlow->setStyleSheet(ConstInfo::NO_COLOR);
    ui->btFast->setStyleSheet(ConstInfo::NO_COLOR);
}

void MainWindow::lightOff(bool isIRLight)
{
    contrState->setAllLightOff(isIRLight);
    for (int i=0; i<5; i++){
        setLight(i, 0, isIRLight);
    }

    cntrlMsgSendThread->sendRightNow(); ;// recalcTimePeriod();
}

void MainWindow::setLight(int lightId, int lightValue,bool isIRLight)
{
    qDebug()<< "Open - lightId : " << lightId << "lightValue : " << lightValue << "isIRLight : " << isIRLight;

    contrState->setLight(lightId, lightValue, isIRLight);

    if (!isIRLight) // обычный свет
    {
        switch (lightId) {
        case ConstInfo::FRONT_LIGHT:
            FuncUtils::setLightImage(ui->lbFront, lightValue);
            break;
        case ConstInfo::REAR_LIGHT:
            FuncUtils::setLightImage(ui->lbRear, lightValue);
            break;
        case ConstInfo::ARM1_LIGHT:
            FuncUtils::setLightImage(ui->lbArm1, lightValue);
            break;
        case ConstInfo::ARM2_LIGHT:
            FuncUtils::setLightImage(ui->lbArm2, lightValue);
            break;
        default:
            break;
        }
    }
    else // инфракрасный свет
    {
        switch (lightId) {
        case ConstInfo::FRONT_LIGHT:
            FuncUtils::setLightIRImage(ui->lbFrontIR, lightValue);
            break;
        case ConstInfo::REAR_LIGHT:
            FuncUtils::setLightIRImage(ui->lbRearIR, lightValue);
            break;
        case ConstInfo::ARM1_LIGHT:
            FuncUtils::setLightIRImage(ui->lbArm1IR, lightValue);
            break;
        case ConstInfo::ARM2_LIGHT:
            FuncUtils::setLightIRImage(ui->lbArm2IR, lightValue);
            break;
        default:
            break;
        }
    }

    cntrlMsgSendThread->sendRightNow();
}

void MainWindow::doItAgain(QString str)
{
    //ui->lbTestThread->setText(str);
}


void MainWindow::onTalk()
{
    ui->label_talk->setVisible(true);
    ui->label_listen_on->setVisible(true);

    initializeAudio();

    /*m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(ConstInfo::IP_ADDRESS, ConstInfo::PORT_SEND_MSG_AUDIO);
    connect(m_pTcpSocket, SIGNAL(connected()), this,  SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this,     SLOT(slotError(QAbstractSocket::SocketError)));

    //connect(m_ptxtInput, SIGNAL(returnPressed()), this, SLOT(slotSendToServer()));
    connect(tmr, SIGNAL(timeout()), this, SLOT(slotSendToServer()));
    connect(m_pTcpSocket, SIGNAL(slotReadyRead()), this, SLOT(slotSendToServer()));*/

   /* m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any, 5212)) {
        QMessageBox::critical(0,
                              "Server Error",
                              "Unable to start the server:"
                              + m_ptcpServer->errorString()
                             );
        m_ptcpServer->close();
        return;
    }
    else qDebug() << "Server 5212 - OPEN!";
    connect(m_ptcpServer, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));

    m_ptxt = new QTextEdit;
    m_ptxt->setReadOnly(true);*/

    m_iVolume = 75;

    /*if  ( ui->label_talk->isVisible()){
        ui->label_listen->setVisible(false);
        ui->label_talk->setVisible(false);
    }
        else
            ui->label_talk->setVisible(true);*/

}
void MainWindow::onListen()
{
    ui->label_talk->setVisible(false);
    ui->label_listen_on->setVisible(false);

    /*disconnect(m_pTcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    m_pTcpSocket->deleteLater();*/
    qDebug() << "Socket 5212 - CLOSED!";

    m_iVolume = 0;
    /*if  ( ui->label_listen->isVisible()){
      ui->label_listen->setVisible(false);
      ui->label_talk->setVisible(false);
    }
        else
            ui->label_listen->setVisible(false);*/

}

void MainWindow::errorConnection(int port)
{
    ui->btStopMove->setEnabled(false);
    ui->btFast->setEnabled(false);
    ui->btSlow->setEnabled(false);
    ui->btCamera->setEnabled(false);
    ui->btCamera_2->setEnabled(false);
    ui->btAudio->setEnabled(false);
    ui->btLight->setEnabled(false);
    ui->btLightIR->setEnabled(false);
    changeSignalLevelRob(0);

    qDebug()<<" MainWindow::errorConnection " << port;
    switch (port) {
    case ConstInfo::PORT_SEND_MSG:
        isSendConnected = false;
        // 20.03.2017
        if (startMedia) {
            isVideoConnected = false;
            _player->stop();
            _player_2->stop();
            videoThr->lostConnection();
            videoThr->socket->disconnectFromHost();
            startMedia = false;
        }
        //
        break;
    case ConstInfo::PORT_RECEIVE_MSG:
        isReceiveConnected = false;
        break;
    case ConstInfo::PORT_VIDEO:
        isVideoConnected = false;
        _player->stop();
        _player_2->stop(); //возможно не надо???
        videoThr->lostConnection();
        break;
    default:
        break;
    }
    ui->label_radiomodem->setEnabled(isSendConnected && isVideoConnected);
    //ui->lbRecordFile->setText(trUtf8("ИДЕТ СОЕДИНЕНИЕ С РОБОТОМ!"));
    ui->infoLabel->setText(trUtf8("ИДЕТ СОЕДИНЕНИЕ С РОБОТОМ!"));

    /*on_btStopMove_clicked();
    on_btStopMove_clicked();*/
    //tmr->stop();
    ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_0"));
    /*serialPort->close();
    serialPort->open(QIODevice::ReadWrite);*/
}

void MainWindow::sendData(QByteArray data){

    int number =   m_pTcpSocket->write(data);
    qDebug() << "Number " << QString::number(number);

}


void MainWindow::isConnected(int port)
{
    qDebug()<<" MainWindow::isConnected " << port;
    switch (port) {
    case ConstInfo::PORT_SEND_MSG:
        isSendConnected = true;
        break;
    case ConstInfo::PORT_RECEIVE_MSG:
        isReceiveConnected = true;
        break;
    case ConstInfo::PORT_VIDEO:
        isVideoConnected = true;
        videoThr->restoreConnection();
        break;
    default:
        break;
    }

    ui->label_radiomodem->setEnabled(isSendConnected && isReceiveConnected && isVideoConnected);
//    ui->lbRecordFile->setText(trUtf8("ГОТОВ К РАБОТЕ!"));
    ui->infoLabel->setText(trUtf8("ГОТОВ К РАБОТЕ!"));
    ui->btStopMove->setEnabled(true);
    ui->btFast->setEnabled(true);
    ui->btSlow->setEnabled(true);
    ui->btCamera->setEnabled(true);
    ui->btCamera_2->setEnabled(true);
    ui->btLight->setEnabled(true);
    ui->btLightIR->setEnabled(true);
    ui->btAudio->setEnabled(false);
    changeSignalLevelRob(4);
    //ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_4"));

    on_btStopMove_clicked();
    on_btStopMove_clicked();
    tmr->start();
}

void MainWindow::on_btArm456_clicked()
{
    contrState->setArmMode(ARM_456);
    changeLightIndicatorArm();
}

void MainWindow::on_btArm234_clicked()
{
    contrState->setArmMode(ARM_234);
    changeLightIndicatorArm();
}

void MainWindow::on_btArm123_clicked()
{
    contrState->setArmMode(ARM_123);
    changeLightIndicatorArm();
}

void MainWindow::on_btFast_clicked()
{
    if(isStop){
        on_btStopMove_clicked();
        isTwoJoystick = false; //isTwoJoystick = false;
        setIndicatorArm();
    }

    //ui->lbRecordFile->setText(trUtf8("ВКЛЮЧЕН РЕЖИМ ДВИЖЕНИЯ РОБОТА!"));

 /*   ui->label_left->setStyleSheet("QLabel { color :"+ConstInfo::NOWORK_COLOR+" }");
    ui->label_right->setStyleSheet("QLabel { color :"+ConstInfo::NOWORK_COLOR+" }");
*/
    contrState->setButSpeed(ConstInfo::FAST_MODE);
    contrState->setMoveMode(MOVE_FAST);
    changeLightIndicatorMove();
    modeMove = MOVE_FAST;

    qba_cc[0] = (char) 0x00;
}

void MainWindow::on_btSlow_clicked()
{
    if (isStop) {
        on_btStopMove_clicked();
        isTwoJoystick = false; //isTwoJoystick = false;
        setIndicatorArm();
    }

    qba_cc[0] = (char) 0x00;

    //ui->lbRecordFile->setText(trUtf8("ВКЛЮЧЕН РЕЖИМ ДВИЖЕНИЯ РОБОТА!"));

  /*  ui->label_left->setStyleSheet("QLabel { color :"+ConstInfo::NOWORK_COLOR+" }");
    ui->label_right->setStyleSheet("QLabel { color :"+ConstInfo::NOWORK_COLOR+" }");
*/
    contrState->setButSpeed(ConstInfo::SLOW_MODE);
    contrState->setMoveMode(MOVE_SLOW);
    changeLightIndicatorMove();
    modeMove = MOVE_SLOW;

    /*isTwoJoystick = false;
    cntrlMsgSendThread->setIsTwoJoystick(false);

    contrState->setAxisForwardBackward((qint8) 0);
    contrState->setAxisLeftRight((qint8) 0);
    contrState->setAxisRotation((qint8) 0);

    setInterfaceTwoJoystick(false);*/
}

void MainWindow::updateTime_my()
{
    if(ui->progressBar_pl1->value() <= 20){
        ui->progressBar_pl1->setStyleSheet(color_r);
        ui->progressBar_pl1->setAlignment(Qt::AlignCenter);
    }
    if(ui->progressBar_pl1->value() > 20 && ui->progressBar_pl1->value() < 50){
        ui->progressBar_pl1->setStyleSheet(color_y);
        ui->progressBar_pl1->setAlignment(Qt::AlignCenter);
    }
    if(ui->progressBar_pl1->value() >= 50){
        ui->progressBar_pl1->setStyleSheet(color_g);
        ui->progressBar_pl1->setAlignment(Qt::AlignCenter);
    }
    if(ui->progressBar_rbt->value() <= 20){
        ui->progressBar_rbt->setStyleSheet(color_r);
        ui->progressBar_rbt->setAlignment(Qt::AlignCenter);
    }
    if(ui->progressBar_rbt->value() > 20 && ui->progressBar_rbt->value() < 50){
        ui->progressBar_rbt->setStyleSheet(color_y);
        ui->progressBar_rbt->setAlignment(Qt::AlignCenter);
    }
    if(ui->progressBar_rbt->value() >= 50){
        ui->progressBar_rbt->setStyleSheet(color_g);
        ui->progressBar_rbt->setAlignment(Qt::AlignCenter);
    }

    //qDebug() << " - " << qbaForMrk.size();
    /*if(qbaForMrk.size() == 20){
        QByteArray buff_;
        buff_.append(qbaForMrk);
        qbaForMrk.prepend(qba_cc);

        QByteArray buff;
        buff.append(buff_[2]);
        buff.append(buff_[3]);
        buff.append(buff_[4]);
        buff.append(buff_[5]);
        buff.append(buff_[6]);
        buff.append(buff_[7]);
        //qDebug() << buff.toHex().toUpper();

        qDebug() << buff.toHex().toUpper() << " : " << qbaForMrk.size();
        buff.clear();
        buff_.clear();
    //qbaForMrk.clear();
    }*/

    /*static int i_U;
    i_U = cntrlMsgSendThread->batt_U_rob();
    if(i_U >0){
        changeBatteryLevelRob(i_U);
    }*/

    static int i_done;
    i_done = cntrlMsgSendThread->done();
    if(i_done == 100){
        qba_cc[0] = (char) 0x00;

        ui->btStopMove->setEnabled(true);
        ui->btFast->setEnabled(true);
        ui->btSlow->setEnabled(true);
       // ui->btCamera->setEnabled(true);
        ui->btCamera_2->setEnabled(true);
        ui->btLight->setEnabled(true);
        ui->btLightIR->setEnabled(true);
        //ui->btAudio->setEnabled(false);
        ui->btVideo->setEnabled(true);

        //on_btSlow_clicked();
        ui->infoLabel->setText(trUtf8("МАНИПУЛЯТОР СЛОЖЕН!"));
    }
    //qDebug() << "i_done : " << i_done << qba_cc.toHex();

    //changeBatteryLevelRob(counter_my);

    int varValue;
    varValue = counter_my * 5;
    ++counter_my;

    if(counter_my == 50){
        pingmodel->start_command();
    }

    if(counter_my % 10 == 0){
        //pingmodel->start_command();
        int my_ping = pingmodel->ping_bullet();
        int i_for_signal_level = 0;
        QString res;
        if(my_ping > 0 && my_ping <= 50){
            i_for_signal_level = 5;
            res = "100%";
        }
        if(my_ping > 50 && my_ping <= 100){
            i_for_signal_level = 4;
            res = "80%";
        }
        if(my_ping > 100 && my_ping <= 150){
            i_for_signal_level = 3;
            res = "60%";
        }
        if(my_ping > 150 && my_ping <= 175){
            i_for_signal_level = 2;
            res = "40%";
        }
        if(my_ping > 175 && my_ping <= 200 ){
            i_for_signal_level = 1;
            res = "20%";
        }
        if(my_ping == 0 || my_ping > 200 ){
            i_for_signal_level = 0;
            res = "0%";
        }
        //qDebug() << "ping = " << my_ping << " - " << res;

        changeSignalLevelRob(i_for_signal_level);
    }

    //qDebug() << "counter_my : " << ++counter_my << " : " << varValue;
    if(counter_my < 50){
        ui->btCamera_2->setEnabled(false);
    }
    else{
        ui->btCamera_2->setEnabled(true);
    }

    /*int angle_0 = cntrlMsgSendThread->angle_zero_ogr();
    int angle_1 = cntrlMsgSendThread->angle_one_ogr();
    int angle_2 = cntrlMsgSendThread->angle_two_ogr();
    int angle_3 = cntrlMsgSendThread->angle_three_ogr();*/

    /*if ((angle_0 != 0) || (angle_1 != 0) || (angle_2 != 0) || (angle_3 != 0)){
        qDebug() << "angle_0 : " << angle_0 << "angle_1 : " << angle_1 << "angle_2 : " << angle_2 << "angle_3 : " << angle_3;
    }*/

  /*  if (angle_0 != 0) cntrlMsgSendThread->analyseArmJoint(angle_0, 0);
    if (angle_1 != 0) cntrlMsgSendThread->analyseArmJoint(angle_1, 1);
    if (angle_2 != 0) cntrlMsgSendThread->analyseArmJoint(angle_2, 2);
    if (angle_3 != 0) cntrlMsgSendThread->analyseArmJoint(angle_3, 3);
    cntrlMsgSendThread->analyseArmJoint(varValue + 45, 4);
    cntrlMsgSendThread->analyseArmJoint(varValue - 45, 5);*/

    //int l = counter_my;
    //int angle_flippers = ogreView->angle_flippers() * 2 + 270;

    //int angle_flippers = cntrlMsgSendThread->angle_flips_ogr();

    //qDebug() << "angle_flippers : " << angle_flippers;
    //if(angle_flippers > 360) angle_flippers = angle_flippers - 360;

  /*  ogreView->rotateFlippers(-angle_flippers);
    ogreView->rotateTurret(-angle_0);
    ogreView->rotateJoint1(-angle_1);
    ogreView->rotateJoint2(-angle_2);
    ogreView->rotateJoint3(angle_3);*/
   /* ogreView->heelBody(l*7);//(qrand()%50);
    ogreView->tangageBody(l*8);// (qrand()%50);*/

   /* int angle_new = ogreView->angle_for_mw();
    QString angle_str = QString::number(angle_new);*/

    /*ui->label_angle->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    //ui->label_angle_2->setAttribute(Qt::WA_TranslucentBackground)
    ui->label_angle->setText("тангаж " + angle_str + "°");

    angle_new = ogreView->angle_for_mw_2();
    angle_str = QString::number(angle_new);
    ui->label_angle_2->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    //ui->label_angle_2->setAttribute(Qt::WA_OpaquePaintEvent);
    ui->label_angle_2->setText("крен " + angle_str + "°");*/

//    ui->label_angle_3->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    //QString str_3 = QString::number(l*25);
    //qDebug() << "L : " << l << "str_3 : " <<str_3;
    //ui->label_angle_3->setText("башня " + QString::number(l+5) + "°");
//    ui->label_angle_3->setText("башня " + QString::number((angle_0 - 80)<0?0:(angle_0 - 80)) + "°");

//    ui->label_angle_4->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    //ui->label_angle_4->setText("плечо " + QString::number(-l+10) + "°");
//    ui->label_angle_4->setText("плечо " + QString::number((angle_1 +10)<0?0:(angle_1 +10)) + "°");

//    ui->label_angle_5->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    //ui->label_angle_5->setText("локоть " + QString::number(-l+5) + "°");
//    ui->label_angle_5->setText("локоть " + QString::number((angle_2 + 90)<0?0:(angle_2 + 90)) + "°");

//    ui->label_angle_6->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    //ui->label_angle_6->setText("захват " + QString::number(l-5) + "°");
//    ui->label_angle_6->setText("захват " + QString::number((angle_3 + 80)<0?0:(angle_3 + 80)) + "°");

//    ui->label_angle_flippers->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");

   /* if(angle_flippers < 180)
    {
        ui->label_angle_flippers->setStyleSheet("QLabel { color :"+ConstInfo::ERROR_COLOR+" }");*/
       // angle_flippers = -angle_flippers;

  /*  }
    else {
        ui->label_angle_flippers->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
        angle_flippers = 360 - angle_flippers;
    }*/

//    ui->label_angle_flippers->setText("угол флипперов " + QString::number((angle_flippers - 120)<0?0:(angle_flippers - 120)) + "°");

   /* ui->label_angle_8->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
    ui->label_angle_8->setText("крен : " + -l*25 + "°"); */

    //changeBatteryLevelRob(58);

    QDateTime date_time = QDateTime::currentDateTime();
    QString s_date_time = date_time.toString();
    ui->label_for_bar->setText(s_date_time);

    SYSTEM_POWER_STATUS lpSystemPowerStatus;
    GetSystemPowerStatus(&lpSystemPowerStatus);
    //qDebug() << lpSystemPowerStatus.BatteryLifePercent;
    switch ((lpSystemPowerStatus.BatteryFlag)) {
        if ((lpSystemPowerStatus.BatteryLifePercent >=0) || (lpSystemPowerStatus.BatteryLifePercent <=100)){
            ui->progressBar_pl1->setValue(lpSystemPowerStatus.BatteryLifePercent);
            ui->progressBar_pl2->setValue(lpSystemPowerStatus.BatteryLifePercent);
        }
    }
}

void MainWindow::on_btStopMove_clicked()
{
    isStop = !isStop;

    if (isStop) {
        // режим Стоп
        ui->btStopMove->setText("УПРАВЛЕНИЕ \n ДВИЖЕНИЕМ РОБОТА");
        //отправить сигнал на остановку робота
        contrState->setAxisForwardBackward((qint8) 0);
        contrState->setAxisLeftRight((qint8) 0);
        contrState->setAxisRotation((qint8) 0);
        // меняем время, чтобы это быстрее дошло пользователю
        cntrlMsgSendThread->recalcTimePeriod();
        contrState->setMoveMode(MOVE_STOP);
        //ui->btArm->setEnabled(true);
        onTwoJoystick();       

      /*  ui->label_left->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
        ui->label_right->setStyleSheet("QLabel { color :"+ConstInfo::WORK_COLOR+" }");
*/
        //ui->lbRecordFile->setText(trUtf8("ВКЛ. РЕЖИМ УПРАВЛЕНИЯ МАНИПУЛЯТОРОМ!"));
        //ui->infoLabel->setText(trUtf8("ВКЛ. РЕЖИМ УПРАВЛЕНИЯ МАНИПУЛЯТОРОМ!"));

        qba_cc[0] = (char) 0xcc;

        //emit message(trUtf8("ВКЛ. РЕЖИМ УПРАВЛЕНИЯ МАНИПУЛЯТОРОМ!"), ConstInfo::Info);
    }
    else {
        // режим Движение


       /* ui->label_left->setStyleSheet("QLabel { color :"+ConstInfo::NOWORK_COLOR+" }");
        ui->label_right->setStyleSheet("QLabel { color :"+ConstInfo::NOWORK_COLOR+" }");
*/
        //ui->lbRecordFile->setText(trUtf8("ВКЛЮЧЕН РЕЖИМ ДВИЖЕНИЯ РОБОТА!"));
        //ui->infoLabel->setText(trUtf8("ВКЛЮЧЕН РЕЖИМ ДВИЖЕНИЯ РОБОТА!"));

        //emit message(trUtf8("ВКЛЮЧЕН РЕЖИМ ДВИЖЕНИЯ РОБОТА!"), ConstInfo::Info);

        ui->btStopMove->setText("УПРАВЛЕНИЕ \n МАНИПУЛЯТОРОМ");
        ui->btArm->setEnabled(false);
        isTwoJoystick = false; //isTwoJoystick = false;
        cntrlMsgSendThread->setIsTwoJoystick(false);
        contrState->setMoveMode(modeMove);
        setInterfaceTwoJoystick(false);
        qba_cc[0] = (char) 0x00;
//        setVisibleRArmButtons(false);
//        ui->btArm234->setVisible(true);
    }
    changeLightIndicatorMove();
}

void MainWindow::setIndicatorArm()
{
/*
    switch (contrState->getArmMode()) {
    case ARM_123:
         ui->btArm123->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
        if (isTwoJoystick){
             ui->btRArm456->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
             ui->btRArm123->setEnabled(false);
             ui->btRArm456->setEnabled(true);
        }
        break;
    case ARM_234:
         ui->btArm234->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
//         if (isTwoJoystick)
//              ui->btRArm156->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
        break;
    case ARM_456:
        ui->btArm456->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
        if (isTwoJoystick){
             ui->btRArm123->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
             ui->btRArm123->setEnabled(true);
             ui->btRArm456->setEnabled(false);
        }
        break;
    default:
        break;
    }*/

}

void MainWindow::changeLightIndicatorArm()
{
    clearColorFilterArm(); //гасим кнопки руки на экране
    clearColorFilterArmR(); //гасим кнопки руки на экране
    //setLedLigths();
    if (contrState->getMoveMode() != MOVE_FAST) setIndicatorArm();
}

void MainWindow::changeLightIndicatorMove()
{
    // управление световой индикацией режимов движения
    clearColorFilterMove();
    //setLedLigths();
    switch (contrState->getMoveMode()) {
    case MOVE_STOP:
        ui->btStopMove->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::WARNING_MESSAGE_COLOR));
        setIndicatorArm();
        break;
    case MOVE_SLOW:
        ui->btSlow->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::INFO_MESSAGE_COLOR));
        setIndicatorArm();
        break;
    case MOVE_FAST:
        ui->btFast->setStyleSheet(FuncUtils::getQPushButtonColorStr(ConstInfo::ERROR_MESSAGE_COLOR));
        clearColorFilterArm(); //гасим кнопки руки на экране
        clearColorFilterArmR(); //гасим кнопки руки на экране
        break;
    default:
        break;
    }
}

void MainWindow::setLedLigths()
{
    FuncUtils::ledLightsOFF();
    if (contrState->getMoveMode() == MOVE_FAST)
    {
        FuncUtils::pressKey(VK_SCROLL);
        FuncUtils::pressKey(VK_CAPITAL);
    }
    else
    {
        if (contrState->getMoveMode() == MOVE_STOP)
        {
            switch (contrState->getArmMode()) {
            case ARM_123:
                break;
            case ARM_234:
                FuncUtils::pressKey(VK_NUMLOCK);
                break;
            case ARM_456:
                FuncUtils::pressKey(VK_CAPITAL);
                break;

            default:
                break;
            }
        }
        else
        {
            if (contrState->getMoveMode() == MOVE_SLOW)
            {
                switch (contrState->getArmMode()) {
                case ARM_123:
                    FuncUtils::pressKey(VK_NUMLOCK);
                    FuncUtils::pressKey(VK_CAPITAL);
                    break;
                case ARM_234:
                    FuncUtils::pressKey(VK_SCROLL);
                    break;
                case ARM_456:
                    FuncUtils::pressKey(VK_SCROLL);
                    FuncUtils::pressKey(VK_NUMLOCK);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void MainWindow::on_btWindowMode_clicked()
{
   /* if (isMaximizedStayOnTop)
    {*/
        Qt::WindowFlags wf = windowFlags();
        wf ^= Qt::WindowStaysOnTopHint;
        wf ^= Qt::FramelessWindowHint;
        setWindowFlags(wf);
        ui->btConnection->setVisible(true); //ui->btConnection->setVisible(true);
        show();
        isMaximizedStayOnTop = false;
  /*  }
    else
    {
        Qt::WindowFlags wf = windowFlags();
        wf |= Qt::WindowStaysOnTopHint;
        wf |= Qt::FramelessWindowHint;
        setWindowFlags(wf);
        ui->btConnection->setVisible(true);//ui->btConnection->setVisible(false);
        showMaximized();
        isMaximizedStayOnTop = true;
    }*/

    m_process->close();
    pingmodel->finished();
    //delete m_process;
    close();
}

void MainWindow::changeLight(int idLight, int lightLevel, bool isIRLight)
{
    qDebug()<< "Open - idLight :" << idLight << "lightLevel" << lightLevel << "isIRLight" << isIRLight;
// пришло сообщение от робота, меняем картинки света
    if (!isIRLight) // обычный свет
    {
        switch (idLight) {
        case ConstInfo::FRONT_LIGHT:
            FuncUtils::setLightImage(ui->lbFront, lightLevel);
            break;
        case ConstInfo::REAR_LIGHT:
            FuncUtils::setLightImage(ui->lbRear, lightLevel);
            break;
        case ConstInfo::ARM1_LIGHT:
            FuncUtils::setLightImage(ui->lbArm1, lightLevel);
            break;
        case ConstInfo::ARM2_LIGHT:
            FuncUtils::setLightImage(ui->lbArm2, lightLevel);
            break;
        default:
            break;
        }
    }
    else // инфракрасный свет
    {
        switch (idLight) {
        case ConstInfo::FRONT_LIGHT:
            FuncUtils::setLightIRImage(ui->lbFrontIR, lightLevel);
            break;
        case ConstInfo::REAR_LIGHT:
            FuncUtils::setLightIRImage(ui->lbRearIR, lightLevel);
            break;
        case ConstInfo::ARM1_LIGHT:
            FuncUtils::setLightIRImage(ui->lbArm1IR, lightLevel);
            break;
        case ConstInfo::ARM2_LIGHT:
            FuncUtils::setLightIRImage(ui->lbArm2IR, lightLevel);
            break;
        default:
            break;
        }
    }
}

void MainWindow::changeBatteryLevelRob(int batteryLevel)
{
// пришло сообщение от робота, меняем картинки уровня заряда батареи робота
    //ui->progressBar_pl1->setValue(batteryLevel);
    //ui->progressBar_pl2->setValue(72);
    ui->progressBar_rbt->setValue(batteryLevel);
}

void MainWindow::changeSignalLevelRob(int signalLevel)
{
// пришло сообщение от робота, меняем картинки уровня сигнала
    switch (signalLevel) {
    case 0:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_0"));
        break;
    case 1:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_1"));
        break;
    case 2:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_2"));
        break;
    case 3:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_3"));
        break;
    case 4:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_4"));
        break;
    case 5:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_5"));
        break;
    default:
        ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_0"));
        break;
    }
}
void MainWindow::changePositionArm(int idArmJoint, int positionValue)
{
   /* switch (idArmJoint) {
    case 0:
         ogreView->rotateTurret(positionValue);
        break;
    case 1:
         ogreView->rotateJoint1(positionValue);
        break;
    case 2:
         ogreView->rotateJoint2(positionValue);
        break;
    case 3:
        ogreView->rotateJoint3(positionValue);
        break;
    case 4:
        ogreView->heelBody(positionValue);
        break;
    case 5:
        ogreView->tangageBody(positionValue);
        break;
    default:
        break;
    }*/

}
/*
void MainWindow::changeFlippers(int angle)
{
    //ui->lbInfo->setText(QString::number(angle));
    ogreView->rotateFlippers(angle);
}*/

void MainWindow::on_btCamera_clicked()
{
    dgCameraMode *dg = new dgCameraMode(contrState ,ui->btCamera);
    connect(dg, SIGNAL(sendToRobot()), this, SLOT(sendToRobot()));
    connect(dg, SIGNAL(dgSecondVideoShow()), this, SLOT(dgSecondVideoShow()));
    connect(dg, SIGNAL(dgSecondVideoDelete()), this, SLOT(dgSecondVideoDelete()));
    dg->setGeometry(100, 100, 443, 160);
    //dg->setGeometry(830, 815, 443, 160);

    //on_btVideo_clicked();

    //dg->setGeometry(10, 160, 230, 130);
//    connect(dg, SIGNAL(Listen()), this, SLOT(onListen()));
    dg->exec();
}

void MainWindow::on_btCamera_2_clicked()
{
    counter_my = 100;
    ++counter_btvideo_click;
    //qDebug() << "counter_btvideo_click : " << counter_btvideo_click;
    /*qDebug() << "VIDEO : " << counter_btvideo_click;
    qDebug() << "IRECVIDEO : " << iRecVideo;*/

   /* dgCameraMode *dg = new dgCameraMode(contrState ,ui->btCamera_2);
    connect(dg, SIGNAL(sendToRobot()), this, SLOT(sendToRobot()));
    connect(dg, SIGNAL(dgSecondVideoShow()), this, SLOT(dgSecondVideoShow()));
    connect(dg, SIGNAL(dgSecondVideoDelete()), this, SLOT(dgSecondVideoDelete()));
    //dg->setGeometry(10, 160, 230, 130);
    dg->setGeometry(850, 690, 423, 160);
    //    connect(dg, SIGNAL(Listen()), this, SLOT(onListen()));
    dg->exec();*/

    /*QDir dir(ConstInfo::DIR_VIDEOS);
    if(!dir.exists())
        QDir().mkdir(ConstInfo::DIR_VIDEOS);*/

    switch (counter_btvideo_click) {
        case 1:
            //name_file_videos = ConstInfo::DIR_VIDEOS+"/video-record_1.avi";
            name_file_videos = ConstInfo::DIR_VIDEOS + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
            //qDebug() << "name_file_videos : " << name_file_videos + "_p01.avi";
            m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                      " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                      " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p01.avi");
            //m_process->start("ffmpeg.exe -f x11grab -s 1280x1024 -r 25 -i -pix_fmt yuv420p"  + name_file_videos + "_p01.avi");
           // ui->infoLabel->setText("ЗАПИСЬ ВИДЕО ТИПА");
            break;
        case 3:
            name_file_videos = ConstInfo::DIR_VIDEOS + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
            m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                      " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                      " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p02.avi");
            //ui->infoLabel->setText("ЗАПИСЬ ВИДЕО ТИПА 2");
            break;
        case 5:
            name_file_videos = ConstInfo::DIR_VIDEOS + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
            m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                      " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                      " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p03.avi");
        case 7:
            name_file_videos = ConstInfo::DIR_VIDEOS + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
            m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                      " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                      " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p04.avi");
            break;
        case 9:
            name_file_videos = ConstInfo::DIR_VIDEOS + QDateTime::currentDateTime().toString("dd-MM-yyyy_hh-mm-ss");
            m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                                      " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                                      " -r 15 -t 00:15:00 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p " + name_file_videos + "_p05.avi");
            break;
        default:
            break;
    }

    /*if(counter_btvideo_click > 10){
        QMessageBox::warning(
                this,
                "Предупреждение",
                "Видео нельзя записать больше 5 раз\n за один сеанс работы!");
    }*/

    //if (ui->infoLabel->text() == " "){
    //if(ui->btCamera_2->text() == "ВИДЕО"){ // || ui->btCamera_2->text() == "ВЕДЕТСЯ\n ВИДЕО ЗАПИСЬ"){
    if(iRecVideo == 1){
         tmr_2->start();
         //qDebug() << "TEXT : " << ui->infoLabel->text();
         ui->infoLabel->setText(trUtf8("ИДЕТ ЗАПИСЬ ВИДЕО!"));
         //ui->infoLabel lbRecordFile->setText(trUtf8("ОСТАНОВИТЬ\nЗАПИСЬ ВИДЕО"));
         //ui->btVideo->setText(trUtf8("ВИДЕО\nСТОП"));
         ui->btCamera_2->setText(trUtf8(" ОСТАНОВИТЬ\nЗАПИСЬ ВИДЕО"));

         //m_process = new QProcess(this);

        /*QDir dir(ConstInfo::DIR_VIDEOS);
        if(!dir.exists())
             QDir().mkdir(ConstInfo::DIR_VIDEOS);*/
         //QString name_file_videos=ConstInfo::DIR_VIDEOS+"/video-record_1.avi"; //+QDateTime::currentDateTime().toString("dd-MM-yy_hh-mm-ss")+".avi";


       /*  m_process->start("ffmpeg.exe -y -rtbufsize 100M -f gdigrab -framerate 15"
                          " -probesize 10M -draw_mouse 0 -i desktop -c:v libx264"
                          " -r 15 -preset ultrafast -tune zerolatency -crf 50 -pix_fmt yuv420p " + name_file_videos);*/


        /* m_process->start("ffmpeg -i ""rtsp://"
                          +ConstInfo::IP_ADDRESS+":"
                           +QString::number(ConstInfo::PORT_VIDEO)+ConstInfo::PATH_VIDEO
                           +""" -vcodec copy -acodec copy -y "+name_file_videos);*/
    }
    //else {
    if(iRecVideo == 0){
        tmr_2->stop();
        counter_my_2 = 0;
        //ui->infoLabel->setText("ЗАПИСЬ ВИДЕО ОСТАНОВЛЕНА ");
        ui->btCamera_2->setText(trUtf8("ВИДЕО"));
        //system("pkill ffmpeg.exe");
        //system("taskkill /im video-record.avi /f");
        //m_process.waitForFinished();
        m_process->close();
        //delete m_process;
    }

     if (!startMedia)
     {
         QMessageBox::warning(
             this,
             "Предупреждение",
             "Видео еще не запущено" );
      }
}

void MainWindow::setBatteryLevel(int batteryLevel){
    ui->progressBar_pl1->setValue(batteryLevel);
    //ui->progressBar_pl2->setValue(72);
  //  ui->progressBar_rbt->setValue(batteryLevel);
}

void MainWindow::on_btConnection_clicked()
{
    // установка связи
    //cntrlMsgRcvThread->reconnect();
    //cntrlMsgSendThread->reconnect();
    videoThr->reconnect();
    //m_pTcpSocket->connectToHost(ConstInfo::IP_ADDRESS, ConstInfo::PORT_SEND_MSG_AUDIO);
    //changeSignalLevelRob(5);

    //Код для отслежевания отмотки кабеля на катушке
//    m_pTcpSocket->connectToHost(, 5210);
}

void MainWindow::on_btPhoto_clicked()
{
    QDir dir(ConstInfo::DIR_PHOTOS);

    if (!dir.exists())
        QDir().mkdir(ConstInfo::DIR_PHOTOS);
    QString name_file = ConstInfo::DIR_PHOTOS+"/photo-"+QDateTime::currentDateTime().toString("dd-MM-yy_hh-mm-ss")+".png";

    QPixmap px = QPixmap::grabWindow(QApplication::desktop()->winId()); // получаем картинку
    px.save(name_file); // сохраняем ее
/*
    ui->btStopMove->setEnabled(true);
    ui->btFast->setEnabled(true);
    ui->btSlow->setEnabled(true);*/
    ui->btAudio->setEnabled(false);
    //ui->btCamera->setEnabled(true);
    //ui->label_signal_level->setPixmap(QPixmap(":/Resources/signal_level_4"));
}

void MainWindow::sendToRobot()
{
    cntrlMsgSendThread->sendRightNow();
}

void MainWindow::on_btArm_clicked()
{
    dgArm *dg = new dgArm(isTwoJoystick,ui->btArm);
    connect(dg, SIGNAL(OneJoystick()), this, SLOT(onOneJoystick()));
    connect(dg, SIGNAL(TwoJoystick()), this, SLOT(onTwoJoystick()));
    dg->exec();

}

void MainWindow::onOneJoystick()
{
    isTwoJoystick = false;
    cntrlMsgSendThread->setIsTwoJoystick(false);

    contrState->setAxisForwardBackward((qint8) 0);
    contrState->setAxisLeftRight((qint8) 0);
    contrState->setAxisRotation((qint8) 0);

    setInterfaceTwoJoystick(false);
}

void MainWindow::onTwoJoystick()
{
   // if (contrState->getArmMode() == ARM_234) contrState->setArmMode(ARM_123);
    isTwoJoystick = true;
    cntrlMsgSendThread->setIsTwoJoystick(true);
    setInterfaceTwoJoystick(true);
   // contrState->setAxisLeftRight(4); // для отладки
}

void MainWindow::setVisibleRArmButtons(bool visible)
{
//    ui->btRArm123->setVisible(visible);
//    ui->btRArm156->setVisible(false);
//    ui->btRArm456->setVisible(visible);
    //ui->frRArm->setVisible(visible);
}

void MainWindow::setInterfaceTwoJoystick(bool twoJoystick)
{
    if (twoJoystick){
        setIndicatorArm();
        ui->btArm234->setVisible(false);
        setVisibleRArmButtons(true);
        ui->btArm123->setFixedHeight(98);
        ui->btArm456->setFixedHeight(98);
        ui->btArm123->move(0, 100);

    }
    else {
        clearColorFilterArmR();
        //ui->btArm234->setVisible(true);
        setVisibleRArmButtons(false);
        ui->btArm123->setFixedHeight(65);
        ui->btArm456->setFixedHeight(65);
        ui->btArm123->move(0, 130);
    }

}

void MainWindow::dgSecondVideoShow()
{
    if (isVideoConnected){

        dgSecondVideo = new dgVLC(ui->btWindowMode);
        dgSecondVideo->show();
        dgSecondVideo->show_video(media2);
        //dgSecondVideo = new dgVLC(ui->btWindowMode);
        //dgSecondVideo = new dgVLC(ui->btCamera_2);*/
        //dgSecondVideo->setGeometry(720, 240, 300, 230);

        //dgSecondVideo->setGeometry(530, 40, 500, 430);

    }
    on_btCamera_clicked();
    //on_btCamera_2_clicked();
}

void MainWindow::dgSecondVideoDelete()
{
    delete dgSecondVideo;
    dgSecondVideo = NULL;
    raise();
}

void MainWindow::receiveMedia2(VlcMedia  * vlcMedia)
{
    media2 = vlcMedia;
}

void MainWindow::reconnect()
{
    if (_player_2->state() == 6 || _player_2->state() == 7) // если ended или error
    {
        _player_2->open(_media_2);
    }
    /*serialPort->close();
    serialPort->open(QIODevice::ReadWrite);*/
}

/*void MainWindow::reconnect_port(){
    if(!serialPort){
        serialPort->close();
        serialPort->open(QIODevice::ReadWrite);
    }
}*/

void MainWindow::errorSerial(QSerialPort::SerialPortError err){
    if(err != 0){
        //qDebug() << "Error : " << err;
        emit signalErrComPort(err);
    }
}
/*
bool MainWindow::isOpen(){
    return serialPort->isOpen();
}

bool MainWindow::open(void){
    try{
        serialPort->setPortName("COM7");
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setParity(QSerialPort::Parity::NoParity);
        serialPort->setDataBits(QSerialPort::DataBits::Data8);
        serialPort->setStopBits(QSerialPort::StopBits::OneStop);
        serialPort->open(QSerialPort::OpenModeFlag::ReadWrite);

        serialPort->open(QIODevice::ReadWrite);
        serialPort->clear();
        serialPort->setDataTerminalReady(true);
        serialPort->setRequestToSend(true);
        serialPort->pinoutSignals();
    }
    catch(...){
        return false;
    }

    return true;
}

bool MainWindow::close_port(){
    if(serialPort->isOpen()){
        serialPort->close();
        return true;
    }
    else{
        return false;
    }
}*/

void MainWindow::on_pushButton_clicked()
{
    QByteArray data;
    data.clear();
    data.append(0x01);
    data.append(0x01);
    data.append(0x01);
    data.append(0x01);
    data.append(0x01);
    data.append(0x01);
    data.append(0x01);
    data.append(0x01);
    emit send(data);

}



void MainWindow::on_pushButton_3_clicked()
{
    QByteArray data;
    int a = 0;
    data.clear();
    data.append(a);
    data.append(a);
    data.append(a);
    data.append(a);
    data.append(a);
    data.append(a);
    data.append(a);
    data.append(a);
    emit send(data);
}
