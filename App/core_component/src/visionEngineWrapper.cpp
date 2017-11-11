#include "visionEngineWrapper.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
cv::Mat g_frame;

VisionEngineWrapper::~VisionEngineWrapper(){
	rythm.stop();
}

VisionEngineWrapper::VisionEngineWrapper() :
		rythm(this) {
	m_visionEngine = VisionEngine::getInstance();
//	m_visionEngine->addTracker("KCF", 0);
	m_video = m_visionEngine->getVidCapture();
	m_debugWinEnabled = false;

	QObject::connect(&rythm, &QTimer::timeout, this,
			&VisionEngineWrapper::worker);

	bbox = new cv::Rect2d(80, 23, 80, 150);

	rythm.start(0);
}

//void VisionEngineWrapper::startTracker() {
//
//	m_visionEngine->startAllTrackers();
//}

void VisionEngineWrapper::stopTracker() {
	m_visionEngine->stopAllTrackers();
}


void VisionEngineWrapper::slot_debugWindowClicked(bool switched) {
m_debugWinEnabled = (m_debugWinEnabled == false) ? true : false;
}

void VisionEngineWrapper::worker() {
	if (m_visionEngine->getVidOpened() && m_debugWinEnabled) {
		m_video.read(g_frame);
	    cv::rectangle(g_frame, *bbox, cv::Scalar(255, 0, 0), 2, 1);

		//TODO: run processing

	    //update debug window
		emit sig_notifyDebugWindow(g_frame);
	}

}


void VisionEngineWrapper::calibrateROI(){
	 QFuture<void> future = QtConcurrent::run(getROI);
	    qDebug() << "hello from visionEngWraper thread " << QThread::currentThread();
	    future.waitForFinished();
//	getROI(g_frame);
}

/**
 * Slot responsible for setting bounding box, to start tracker.
 * @param keyCode - Qt::Key code.
 */
void VisionEngineWrapper::slot_keyHandler(int keyCode){

	switch(keyCode){
	case 68:	// "D"
		bbox->x+=5;
		break;
	case 65:	// "A"
		bbox->x-=5;
		break;
	case 87:	// "S"
		bbox->y-=5;
		break;
	case 83:	// "W"
		bbox->y+=5;
	}
}
