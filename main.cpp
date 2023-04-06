#include <QApplication>
#include "mainwindow.h"

#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    QNetworkConfigurationManager m_config;
   // QNetworkConfiguration q = m_config.defaultConfiguration();
    foreach (const QNetworkConfiguration& netConfig, m_config.allConfigurations()) {
        qDebug() << netConfig.name();
//      if (netConfig.bearerType() == QNetworkConfiguration::BearerWLAN) {
//        // I'm wireless network
//        if (netConfig.type() == QNetworkConfiguration::InternetAccessPoint) {
//          // I'm access point
//        }
//      }
    }

    MainWindow w(argc, argv);
    w.showMaximized();
   // w.show();

    return a.exec();
}//some changes in maksimsBranch
//second changes in maksimsBranch
