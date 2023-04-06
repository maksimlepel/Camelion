#ifndef MODETYPES_H
#define MODETYPES_H

#include <QMap>
#include <Utils/ConstInfo.h>

enum enArmMode
{
    ARM_123 = 0,
    ARM_234 = 1,
    ARM_456 = 2
};

typedef enArmMode ArmMode;

inline ArmMode& operator++(ArmMode& m, int) {
    if (m == ARM_123)
    {
        m = ARM_234;
    }
    else
        if (m == ARM_234)
        {
            m = ARM_456;
        }
        else
            if (m == ARM_456)
            {
                m = ARM_123;
            }
            else m = ARM_123; // на всякий пожарный
    return m;
}


enum enMoveMode
{
    MOVE_STOP = 0,
    MOVE_SLOW = 1,
    MOVE_FAST = 2
};

typedef enMoveMode MoveMode;

inline MoveMode& operator++(MoveMode& m, int) {
    if (m == MOVE_STOP)
    {
        m = MOVE_SLOW;
    }
    else
        if (m == MOVE_SLOW)
        {
            m = MOVE_FAST;
        }
        else
            if (m == MOVE_FAST)
            {
                m = MOVE_STOP;
            }
            else m = MOVE_SLOW; // на всякий пожарный
    return m;
}


// режимы Камеры
enum enCameraMode
{
    cmMono = 1,   // одна камера
    cmPiP = 0,    // картинка в картинке
    cmMosaic = 2 // мозаика
};

typedef enCameraMode CameraMode;

enum enCamera
{
    CAMERA_1 = 1,
    CAMERA_2 = 2,
    CAMERA_3 = 3,
    CAMERA_4 = 4,
    CAMERA_5 = 5,
    CAMERA_6 = 6
};

typedef enCamera Camera;

class CameraProtitype
{
public:
    CameraProtitype(){
    }

    Camera getCamera(){
        return c;
    }

    void setCurrentCamera(Camera camera){
        c = camera;
    }

    QList<Camera> getPossibleCamera(){
        return lPossibleCamera;
    }

    void operator++(int m) {
        int rez = lPossibleCamera.indexOf(c);
        if (rez == (lPossibleCamera.count()-1)) {
            c = lPossibleCamera.at(0);
        }
        else
            if (rez  == -1 ){
                return;
            }
            else {
                c = lPossibleCamera.at(rez + 1);
            }
    }

protected:
    Camera c;
    QList<Camera> lPossibleCamera;
};


class CameraMono: public CameraProtitype
{
public:
    CameraMono(){
        lPossibleCamera << CAMERA_1;
        lPossibleCamera << CAMERA_2;
        lPossibleCamera << CAMERA_3;
        lPossibleCamera << CAMERA_4;
        lPossibleCamera << CAMERA_5;
//        lPossibleCamera << CAMERA_6;
        c = CAMERA_1;
    }

};

typedef CameraMono CameraBig;
typedef CameraMono CameraSmall;

class CameraPip
{
public:
    CameraPip(){
        cBig.setCurrentCamera(CAMERA_1);
        cSmall.setCurrentCamera(CAMERA_2);
    }

    CameraBig cBig;
    CameraSmall cSmall;

    bool checkBig(){
        bool rez = true;
        if (cBig.getCamera() == cSmall.getCamera()) {
            rez = false;
            cSmall++;
        }
        return rez;
    }

    bool checkSmall(){
        bool rez = true;
        if (cBig.getCamera() == cSmall.getCamera()) {
            rez = false;
            cBig++;
        }
        return rez;
    }
};

class CameraTopLeft: public CameraProtitype
{
public:
    CameraTopLeft(){
        lPossibleCamera << CAMERA_1;
        lPossibleCamera << CAMERA_2;
        c = CAMERA_1;
    }
};

class CameraTopRight: public CameraProtitype
{
public:
    CameraTopRight(){
        lPossibleCamera << CAMERA_2;
        lPossibleCamera << CAMERA_3;
        c = CAMERA_2;
    }
};

class CameraBottomLeft: public CameraProtitype
{
public:
    CameraBottomLeft(){
        lPossibleCamera << CAMERA_3;
        lPossibleCamera << CAMERA_4;
        c = CAMERA_3;
    }
};

class CameraBottomRight: public CameraProtitype
{
public:
    CameraBottomRight(){
        lPossibleCamera << CAMERA_4;
        lPossibleCamera << CAMERA_5;
        lPossibleCamera << CAMERA_6;
        c = CAMERA_4;
    }
};

class CameraMosaic
{
public:
    CameraMosaic(){
        cTopLeft.setCurrentCamera(CAMERA_1);
        cTopRight.setCurrentCamera(CAMERA_2);
        cBottomLeft.setCurrentCamera(CAMERA_3);
        cBottomRight.setCurrentCamera(CAMERA_4);
    }


    CameraTopLeft cTopLeft;
    CameraTopRight cTopRight;
    CameraBottomLeft cBottomLeft;
    CameraBottomRight cBottomRight;

    bool checkTopLeft(){
        bool rez = true;
        if (cTopLeft.getCamera() == cTopRight.getCamera()) {
            rez = false;
            cTopRight++;
            checkTopRight();
        }
        return rez;
    }

    bool checkTopRight(){
        bool rez = true;
        if (cTopRight.getCamera() == cTopLeft.getCamera()) {
            rez = false;
            cTopLeft++;
            checkTopLeft();
        }
        if (cTopRight.getCamera() == cBottomLeft.getCamera()) {
            rez = false;
            cBottomLeft++;
            checkBottomLeft();
        }
        return rez;
    }

    bool checkBottomLeft(){
        bool rez = true;
        if (cBottomLeft.getCamera() == cTopRight.getCamera()) {
            rez = false;
            cTopRight++;
            checkTopRight();
        }
        if (cBottomLeft.getCamera() == cBottomRight.getCamera()) {
            rez = false;
            cBottomRight++;
            checkBottomRight();
        }
        return rez;
    }

    bool checkBottomRight(){
        bool rez = true;
        if (cBottomRight.getCamera() == cBottomLeft.getCamera()) {
            rez = false;
            cBottomLeft++;
            checkBottomLeft();
        }
        return rez;
    }
};

#endif // MODETYPES_H
