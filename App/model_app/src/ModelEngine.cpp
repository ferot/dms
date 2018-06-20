#include <QThread>
#include <QFuture>
#include <QtConcurrent/qtconcurrentrun.h>

#include <future>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/core/ocl.hpp>

#include "ModelEngine.h"
#include "CoordRcvdCmd.hpp"

cv::Mat g_frame;
fann_type input[9]={0};


ModelEngine::~ModelEngine(){
	fann_destroy(m_ann);
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
	} else {
		LOGMSG(LOG_ERROR, "Couldn't start event reader thread. Exitting...");
		exit(1);
	}

	QObject::connect(&rythm, &QTimer::timeout, this,
			&ModelEngine::worker);

	std::string annFilePath = Config::getInstance()->getValue("ANN", "net");
	if(loadNetFile(annFilePath) == CMN_RC_ERROR){
		LOGMSG(LOG_ERROR, "Couldn't load definition file!");
        abort();
	}


	m_scaleFactorX = std::stof(Config::getInstance()->getValue("Video", "width"));
	m_scaleFactorY = std::stof(Config::getInstance()->getValue("Video", "height"));

	m_scaleMap.insert(std::pair<int,float>(scaleID::X, m_scaleFactorX));
	m_scaleMap.insert(std::pair<int,float>(scaleID::Y, m_scaleFactorY));
	//Arbitrary assumed that detected face size couldn't be bigger than half of screen size
	m_scaleMap.insert(std::pair<int,float>(scaleID::SIZE, m_scaleFactorX/2));

	rythm.start(0);
}

void ModelEngine::slot_modelWindowButtonClicked(bool){
	m_modelWinEnabled = (m_modelWinEnabled == false) ? true : false;
}

/**
 * Main processing worker method
 */
void ModelEngine::worker() {

        printCamDebug();

    auto input = obtainInputVec();
    fann_type * result = calculateResult(input);

    float x =((result[0]));
    float y = ((result[1]));
    LOGMSG_ARG(LOG_DEBUG, "ANN RESULT COORDS (X,Y) : %s",
               std::string(
                   std::to_string(x) + " , "
                   + std::to_string(y)).c_str());
    if (m_modelWinEnabled) {
        QThread::msleep(10); //this is unfortunately essential for now due to crash.
        emit sig_notifyModelWindow(std::round(x), std::round(y));
    }
}

void ModelEngine::printCamDebug(){
	LOGMSG_ARG(LOG_DEBUG, "SIZEOF coords %d", m_coords.cam_coords.size());

	for (int i = 0; i < cam_nrs; i++) {
		auto& cam_vec = getCoords(i);

		LOGMSG_ARG(LOG_DEBUG, "CAM %d", i);
		LOGMSG_ARG(LOG_DEBUG, "x %s", cam_vec[0].c_str());
		LOGMSG_ARG(LOG_DEBUG, "y = %s", cam_vec[1].c_str());
		LOGMSG_ARG(LOG_DEBUG, "size = %s", cam_vec[2].c_str());

	}
}

void ModelEngine::setCoords(int id, t_tup_thrstrs tuple) {
	m_coords.setRow(tuple, id);
}

const t_tup_thrstrs& ModelEngine::getCoords(int id) {
	return m_coords.getRow(id);
}

/***
 * Attemts to load generated .net file from provided filepath.
 * @param filepath
 * @return CMN_RC_SUCCESS when struct is not empty
 *
 * Note:  This function may crash app when it fails!!
 */
CommonRC ModelEngine::loadNetFile(std::string filepath) {
	CommonRC ret = CMN_RC_ERROR;
	if ((m_ann = fann_create_from_file(filepath.c_str()))) {
		ret = CMN_RC_SUCCESS;
	}
	return ret;
}

/***
 * Main ANN processing point.
 * @param input vector for network
 * @return result as (x,y) tuple
 */
fann_type* ModelEngine::calculateResult(std::array<float, 9> input){
    fann_scale_input(m_ann, input.data());
    fann_type* result = fann_run(m_ann, input.data());
    fann_descale_output(m_ann, result);

    return result;
}

std::array<float, 9> ModelEngine::obtainInputVec(){
    std::array<float, 9> input;
      int curr_index = 0;

	for (int i = 0; i < cam_nrs; i++) {
		auto& cam_vec = getCoords(i);
		for (int j = 0; j < 3; j++) {
            input[curr_index++] = std::stof(cam_vec[j]);
		}
	}
    return input;
}
