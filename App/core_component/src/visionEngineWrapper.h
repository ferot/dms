#ifndef VISIONENGINEWRAPPER_H
#define VISIONENGINEWRAPPER_H

#include <QObject>
#include <QTimer>

#include "VisionEngine.hpp"
#include "Tracker.hpp"
#include "HaarTracker.hpp"

enum State {
	INIT_S, FIND_TRGT_S, INIT_TRCK_S, RUN_TRCK_S, VERIF_TRGT_S, STOP_S = 0xff
};

class VisionEngineWrapper: public QObject {
	Q_OBJECT
private:
	bool m_trackerInited;
	bool m_trackingEnabled;
	bool m_modelDebugWinEnabled;
	bool m_switchTracker;

	State m_state;

	VisionEngine * m_visionEngine;
	std::shared_ptr<Track::Tracker> m_tracker;
	cv::Ptr<HaarTracker> m_htracker;

	cv::VideoCapture m_video;
	QTimer rythm;

	bool m_debugWinEnabled;
	cv::Mat cameraFrame;
	cv::Rect2d * bbox;

	void disableTracking();
	void worker();
	bool checkObjAtBnd(cv::Mat& frame, t_bBox &bounding);

	void findTargetStateFn(t_bBox&);
	void initTrackStateFn(t_bBox& tr);
	void runTrackStateFn(t_bBox& tr);
	void verifyTargetStateFn(t_bBox& tr);
public:
	t_bBox track();
	explicit VisionEngineWrapper();
	~VisionEngineWrapper();signals:
	void sig_notifyDebugWindow(cv::Mat);
	void sig_notifyModelDebugWindow(cv::Point);

	void sig_selectedROI(cv::Mat);public slots:
	void slot_keyHandler(int);
	void slot_debugWindowClicked(bool);
	void slot_modelDebugWindowClicked(bool);
	void slot_switchTrackerClicked(bool);

	void stopTracker();
};

#endif // VISIONENGINEWRAPPER_H
