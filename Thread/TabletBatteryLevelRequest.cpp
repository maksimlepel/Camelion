#include "TabletBatteryLevelRequest.h"
#include "winbase.h"
#include "QThread"
#include <QTest>


TabletBatteryLevelRequest::TabletBatteryLevelRequest()
{
}

void TabletBatteryLevelRequest::run(){
    SYSTEM_POWER_STATUS lpSystemPowerStatus;
    while (true){
    GetSystemPowerStatus(&lpSystemPowerStatus);
    //qDebug() << lpSystemPowerStatus.BatteryLifePercent;
    switch ((lpSystemPowerStatus.BatteryFlag)) {
  //  case 8:
       // ui->labelInfo->setText("Charging battery");
   //     break;
    case 128:
       // ui->labelInfo->setText("No system battery");
        break;
    case 255:
        //ui->labelInfo->setText("Unknown status");
        break;
    default:
        if ((lpSystemPowerStatus.BatteryLifePercent >=0) || (lpSystemPowerStatus.BatteryLifePercent <=100)){
            emit batteryInfo(lpSystemPowerStatus.BatteryLifePercent);
        }
        else
            emit batteryInfo(0);
        break;
    }
    QTest::qWait(60000);
    }
}

