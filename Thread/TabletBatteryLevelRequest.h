#ifndef TABLETBATTERYLEVELREQUEST_H
#define TABLETBATTERYLEVELREQUEST_H

#include <QtCore/QThread>
#include <QObject>
#include <windows.h>

class TabletBatteryLevelRequest: public QThread
{
    Q_OBJECT
public:
    TabletBatteryLevelRequest();

    void run();

signals:
    void batteryInfo(int batteryLevel);
};

#endif // TABLETBATTERYLEVELREQUEST_H
