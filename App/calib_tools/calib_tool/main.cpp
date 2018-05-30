#include "calibtool.h"
#include <QApplication>
#include "CommunicationEngine.hpp"
#include "DispatchEngine.hpp"
#include "CommEvent.hpp"
#include "Config.hpp"
#include "CoordRcvdCmd.hpp"
#include "TrainJobFinishedCmd.hpp"

int main(int argc, char *argv[])
{
	if (!initLogger()) {
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	} else {
		printf("Logger init failed");
	}

	DispatchEngine *de = DispatchEngine::getInstance();
	CommunicationEngine *ce = CommunicationEngine::getInstance("calib_tool");
	ce->connect();

	std::string topicName = Config::getInstance()->getValue("MQTT", "topic_name");

	CommonRC ret = de->startEventReader();
	if (ret == CMN_RC_SUCCESS) {
		LOGMSG(LOG_DEBUG, "started reader thread");
	}

    QApplication a(argc, argv);
    CalibTool w;
	t_commandPtr coordEvt(new CoordsRcvdCmd(&w));
	t_commandPtr jobFinishedEvt(new TrainJobFinishedCmd(&w));

	ce->subscribe(topicName, eventType::USER_EVENT, coordEvt);
	de->registerEvent(eventType::JOB_FINISHED_EVENT, jobFinishedEvt);

    w.show();

    return a.exec();
}
