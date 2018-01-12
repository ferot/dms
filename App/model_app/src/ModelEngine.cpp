#include "ModelEngine.h"
#include <QThread>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#include <future>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>
#include "CoordRcvdCmd.hpp"

cv::Mat g_frame;


ModelEngine::~ModelEngine(){
	rythm.stop();
}

ModelEngine::ModelEngine() :
		rythm(this) {
	ce = CommunicationEngine::getInstance("model_app");
	de = DispatchEngine::getInstance();

	ce->connect();
	std::string topicName = Config::getInstance()->getValue("MQTT", "topic_name");

	t_commandPtr coordEvt(new CoordsRcvdCmd(this));
	ce->subscribe(topicName, eventType::USER_EVENT, coordEvt);

	CommonRC ret = de->startEventReader();
	if (ret == CMN_RC_SUCCESS) {
		LOGMSG(LOG_DEBUG, "started reader thread");
	}


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

	printCamDebug();

	if (m_modelWinEnabled) {
		QThread::msleep(1); //this is unfortunately essential for now due to crash.

		emit sig_notifyModelWindow(
				cv::Point(trackResult.x + 50, trackResult.y + 50));

	}
}

void ModelEngine::printCamDebug(){
	LOGMSG_ARG(LOG_DEBUG, "SIZEOF coords %d", m_coords.cam_coords.size());

	for (int i = 0; i < cam_nrs; i++) {
		auto& cam_vec = getCoords(i);

		LOGMSG_ARG(LOG_DEBUG, "CAM %d", i);
		if (i == 2)
			i = 0;

		LOGMSG_ARG(LOG_DEBUG, "x %s", cam_vec[0].c_str());
		LOGMSG_ARG(LOG_DEBUG, "y = %s", cam_vec[1].c_str());
		LOGMSG_ARG(LOG_DEBUG, "size = %s", cam_vec[2].c_str());
		i++;

	}
}

void ModelEngine::setCoords(int id, t_tup_thrstrs tuple) {
	m_coords.setRow(tuple, id);
}

const t_tup_thrstrs& ModelEngine::getCoords(int id) {
	return m_coords.getRow(id);
}
