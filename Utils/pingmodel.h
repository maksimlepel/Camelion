#ifndef PINGMODEL_H
#define PINGMODEL_H

#include <QObject>
#include <QProcess>

class PingModel : public QObject
{
    Q_OBJECT
public:
    explicit PingModel(QObject *parent = 0);
    ~PingModel();

    void start_command();
    int ping_bullet();
    bool is_running();
    bool finished();
    int i_ping;

signals:

public slots:
    void verifyStatus();
    void readResult();

private:
    QProcess *ping;
    bool running;
};

#endif // PINGMODEL_H
