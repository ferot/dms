#include "ModelEngine.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

cv::Mat g_frame;


ModelEngine::~ModelEngine(){
	rythm.stop();
}

ModelEngine::ModelEngine() :
		rythm(this) {


	QObject::connect(&rythm, &QTimer::timeout, this,
			&ModelEngine::worker);

	rythm.start(0);
}

void ModelEngine::slot_modelWindowButtonClicked(bool){
	m_modelWinEnabled = (m_modelWinEnabled == false) ? true : false;
}

/**
 * Main processing worker method
 */
void ModelEngine::worker() {

	t_bBox trackResult;

	if (m_modelWinEnabled) {
		QThread::msleep(1); //this is unfortunately essential for now due to crash.

		emit sig_notifyModelWindow(
				cv::Point(trackResult.x + 50, trackResult.y + 50));

	}
}

