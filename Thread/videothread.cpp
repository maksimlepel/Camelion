#include <QDateTime>
#include <QDir>
#include "videothread.h"


VideoThread::VideoThread(int numPort, QObject *parent, VlcInstance *_instance)
    :ThreadPrototype(numPort, parent)
{
    qDebug() << "VideoThread" << numPort;
    moveToThread(this);
    this->_instance=_instance;
    _media = NULL;
    //_media = new  VlcMedia("rtsp://192.168.6.77:8554/video", _instance);
    //_media2 = new  VlcMedia("rtsp://192.168.6.77:8554/video", _instance);
    _media2 = NULL;
}

void VideoThread::mainFunction()
{
    _media = new  VlcMedia("D:/Хакер.avi", _instance);
    _media = new  VlcMedia("rtsp://" + ConstInfo::IP_ADDRESS+":"+QString::number(ConstInfo::PORT_VIDEO)+ConstInfo::PATH_VIDEO2, _instance);
   /* QString url = "rtsp://admin:admin@192.168.6.168";
    _media = new  VlcMedia(url, _instance);*/
    connect(_media, SIGNAL(stateChanged(Vlc::State)), this, SLOT(stateVlcChanged(Vlc::State)));
    emit videoStart(_media);
    _media2 = new  VlcMedia("rtsp://" + ConstInfo::IP_ADDRESS+":"+QString::number(ConstInfo::PORT_VIDEO)+ConstInfo::PATH_VIDEO, _instance);
    //_media2 = new  VlcMedia("rtsp://192.168.6.77:8554/video", _instance);
    connect(_media2, SIGNAL(stateChanged(Vlc::State)), this, SLOT(stateVlcChanged(Vlc::State)));
    emit videoStart2(_media2);
}

void VideoThread::restoreConnection()
{
    if (_media == NULL){
        qDebug()<<"Create vlcMedia";
        //_media = new  VlcMedia("rtsp://192.168.6.77:8554/video", _instance);
        _media = new  VlcMedia("rtsp://" + ConstInfo::IP_ADDRESS+":"+QString::number(ConstInfo::PORT_VIDEO)+ConstInfo::PATH_VIDEO2, _instance);
        connect(_media, SIGNAL(stateChanged(Vlc::State)), this, SLOT(stateVlcChanged(Vlc::State)));
        emit videoStart(_media);
    }

     if (_media2 == NULL){
         //_media2 = new  VlcMedia("rtsp://192.168.6.77:8554/video", _instance);
         _media2 = new  VlcMedia("rtsp://" + ConstInfo::IP_ADDRESS+":"+QString::number(ConstInfo::PORT_VIDEO)+ConstInfo::PATH_VIDEO, _instance);
         connect(_media2, SIGNAL(stateChanged(Vlc::State)), this, SLOT(stateVlcChanged(Vlc::State)));

        emit videoStart2(_media2);
     }

}

void VideoThread::stateVlcChanged(const Vlc::State &state)
{
    if ((state == Vlc::Ended) or (state == Vlc::Error)){
         qDebug() << "VideoThread:: soketState " << socket->state();
         socket->disconnectFromHost();
        //stateChanged(QAbstractSocket::UnconnectedState);
    }
    qDebug() << "VideoThread::stateChanged " << state;
}

void VideoThread::lostConnection()
{
    // при потере соединения убиваем видео
    if (_media != NULL){
        delete _media;
        _media = NULL;
    }
    if (_media2 != NULL){
        delete _media2;
        _media2 = NULL;
    }
}

VideoThread::~VideoThread()
{
    if (_media != NULL){
        delete _media;
        _media = NULL;
    }
    if (_media2 != NULL){
        delete _media2;
        _media2 = NULL;
    }
}
