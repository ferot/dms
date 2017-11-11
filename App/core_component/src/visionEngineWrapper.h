#ifndef VISIONENGINEWRAPPER_H
#define VISIONENGINEWRAPPER_H

#include <QObject>
#include <QTimer>

#include "VisionEngine.hpp"


class VisionEngineWrapper : public QObject
{
    Q_OBJECT
private:
    void worker();
    VisionEngine * m_visionEngine;
    cv::VideoCapture m_video;
    QTimer rythm;

    bool m_debugWinEnabled;
    cv::Mat cameraFrame;
    cv::Rect2d * bbox;
public:
//    void getROI();
    explicit VisionEngineWrapper();
    ~VisionEngineWrapper();
signals:
    void sig_notifyDebugWindow(cv::Mat);
    void sig_selectedROI(cv::Mat);
public slots:
	void slot_keyHandler(int);
    void slot_debugWindowClicked(bool);
    void stopTracker();
};

#endif // VISIONENGINEWRAPPER_H
