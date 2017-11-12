#ifndef VISIONENGINEWRAPPER_H
#define VISIONENGINEWRAPPER_H

#include <QObject>
#include <QTimer>

#include "VisionEngine.hpp"
#include "Tracker.hpp"


class VisionEngineWrapper : public QObject
{
    Q_OBJECT
private:
    bool m_trackerInited;
    bool m_trackingEnabled;
    void worker();
    VisionEngine * m_visionEngine;
    std::shared_ptr<Track::Tracker> m_tracker;
    cv::VideoCapture m_video;
    QTimer rythm;

    bool m_debugWinEnabled;
    cv::Mat cameraFrame;
    cv::Rect2d * bbox;
public:
    t_bBox track();
//    void getROI();
    explicit VisionEngineWrapper();
    ~VisionEngineWrapper();
signals:
    void sig_notifyDebugWindow(cv::Mat);
    void sig_selectedROI(cv::Mat);
public slots:
	void slot_keyHandler(int);
    void slot_debugWindowClicked(bool);
    void slot_trackWindowClicked(bool);

    void stopTracker();
};

#endif // VISIONENGINEWRAPPER_H
