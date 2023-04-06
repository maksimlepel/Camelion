#ifndef SHOWMESSAGE_H
#define SHOWMESSAGE_H

#include <QObject>
#include <QLabel>
#include <QList>
#include <QTimer>
#include "Utils/ConstInfo.h"

// для отображения кратких сообщений, не требующих ответа пользователя, исчезающих через некоторое время

struct MessageStruct
{
    QString message;
    ConstInfo::TypeMessage type;
    inline bool operator==(const MessageStruct& mc)
    {
         return (message == mc.message
                     && type== mc.type
                    );
    }
};

class ShowMessage:public QObject
{
    Q_OBJECT
public:
    explicit ShowMessage(QLabel *lbInfo);
    ~ShowMessage();

public slots:
    void addMessage(QString message, ConstInfo::TypeMessage typeMessage);


private:
    bool isShowNow; // в данный момент показывается сообщение
    QLabel *lbInfo;
    QList<MessageStruct> messageQueue;
    QTimer timer;

    void clear(); // очищаем строку, цвет зеленый
    void draw();

private slots:
    void timeOver();
};

#endif // SHOWMESSAGE_H
