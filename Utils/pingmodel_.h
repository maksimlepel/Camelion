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
    int ping_bullet(int i_p);
    bool is_running();
    bool finished();
    
signals:
    
public slots:
    void verifyStatus();
    void readResult();

private:
    QProcess *ping;
    bool running;
};

#endif // PINGMODEL_H
