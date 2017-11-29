#include "calibtool.h"
#include <QApplication>
#include "CommunicationEngine.hpp"
#include "DispatchEngine.hpp"
#include "CommEvent.hpp"
#include "Config.hpp"


int main(int argc, char *argv[])
{
	if (!initLogger()) {
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	} else {
		printf("Logger init failed");
	}

	CommunicationEngine *ce = CommunicationEngine::getInstance();
	ce->connect();
	std::string topicName = Config::getInstance()->getValue("MQTT", "topic_name");
	ce->subscribe(topicName);

    QApplication a(argc, argv);
    CalibTool w;
    w.show();

    return a.exec();
}
