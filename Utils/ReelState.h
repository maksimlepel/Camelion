#ifndef REELSTATE_H
#define REELSTATE_H
#include <QObject>
#include <QProcess>

class ReelState
{
public:

    ReelState();

    ~ReelState();

    //Getting lengthM and lengthCm of ReelState
    qint8 getLengthM();
    qint8 getLengthCm();

    //Setting lengthM and lengthCm of ReelState
    void setLengthM(qint8 lengthM);
    void setLengthCm(qint8 lengthCm);




private:

    qint8 m_lengthM;
    qint8 m_lengthCm;

};

#endif // REELSTATE_H
