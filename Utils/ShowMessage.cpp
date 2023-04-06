#include "ShowMessage.h"

#include "Utils/FuncUtils.h"

ShowMessage::ShowMessage(QLabel *parent)
    : QObject(0),
      isShowNow(false)
{
    this->lbInfo = parent;
    messageQueue.clear();

    timer.setInterval(ConstInfo::SHOW_MESSAGE_TIME);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeOver()));
}

ShowMessage::~ShowMessage()
{
    timer.stop();

}

void ShowMessage::addMessage(QString message, ConstInfo::TypeMessage typeMessage)
{
    MessageStruct ms;
    ms.message = message;
    ms.type = typeMessage;
    if (!messageQueue.contains(ms))
    {
        messageQueue.append(ms);

    }
    if (!isShowNow) draw();
}

void ShowMessage::draw()
{
    if (!messageQueue.isEmpty())
    {
        if (lbInfo != NULL)
        {
            isShowNow = true;
            lbInfo->setText(messageQueue.first().message);
            lbInfo->setStyleSheet(FuncUtils::getQLabelColorStr(messageQueue.first().type));
            messageQueue.removeFirst();
            timer.start();
        }
    }
    else
    {
        isShowNow = false;
        clear();
    }
}

void ShowMessage::clear()
{
    lbInfo->setText("");
    lbInfo->setStyleSheet(ConstInfo::NO_COLOR/*FuncUtils:: getQLabelColorStr(ConstInfo::Info)*/);
}

void ShowMessage::timeOver()
{
    timer.stop();
    draw();
}
