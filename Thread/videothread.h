#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include "Thread/ThreadPrototype.h"
#include <libvlc-qt/include/VLCQtCore/Common.h>
#include <libvlc-qt/include/VLCQtCore/Instance.h>
#include <libvlc-qt/include/VLCQtCore/Media.h>
#include <libvlc-qt/include/VLCQtCore/MediaPlayer.h>
#include <libvlc-qt/include/VLCQtWidgets/WidgetVideo.h>
#include <libvlc-qt/include/VLCQtCore/Enums.h>

class VideoThread: public ThreadPrototype
{
    Q_OBJECT
public:
    explicit VideoThread(int numPort, QObject *parent, VlcInstance *_instance);
    ~VideoThread();
    void lostConnection();
    void restoreConnection();

protected:
    virtual void mainFunction();

private:
    VlcInstance *_instance;

    VlcMedia  *_media;
    VlcMedia  *_media2;

private slots:
    void stateVlcChanged(const Vlc::State &state);

signals:
    void videoStart( VlcMedia  *);
    void videoStart2( VlcMedia  *);
    void videoStop();

};

#endif // VIDEOTHREAD_H

