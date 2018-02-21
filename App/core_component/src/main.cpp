/*
 * main.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: tf
 */
#include "window.h"

#include <iostream>

#include "main.hpp"

#include "Event.hpp"

#include "CommunicationEngine.hpp"
#include "CommEvent.hpp"
#include "StorageEngine.hpp"
#include "Config.hpp"
#include "NodeEngine.hpp"

#include "DispatchEngine.hpp"

#include <iostream>
#include <thread>
#include <QApplication>
#include <QPushButton>
#include <QObject>
#include "window.h"
#include "Event.hpp"
#include "threader.h"

#include "visionEngineWrapper.h"

using namespace std;

int main(int argc, char **argv) {

	if (!initLogger()) {
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	} else {
		printf("Logger init failed");
	}

	LOGMSG(LOG_DEBUG, "Starting core_app main!");

	CommunicationEngine *ce = CommunicationEngine::getInstance();
	DispatchEngine *de = DispatchEngine::getInstance();

	ce->connect();
	std::string topicName = Config::getInstance()->getValue("MQTT", "topic_name");
	ce->subscribe(topicName);

	QApplication app(argc, argv);
	Threader threader;
	VisionEngineWrapper visionEngine;
	Window mainWindow(visionEngine.getCamID());


	threader.runInThread(&visionEngine);

	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<cv::Point>("cv::Point");

	QObject::connect(&visionEngine, &VisionEngineWrapper::sig_notifyDebugWindow,
			&mainWindow, &Window::slot_updateDebugWindow);

	QObject::connect(&mainWindow, &Window::sig_notifyDebugWindow, &visionEngine,
			&VisionEngineWrapper::slot_debugWindowClicked);

	QObject::connect(&visionEngine, &VisionEngineWrapper::sig_notifyModelDebugWindow,
			&mainWindow, &Window::slot_updateModelDebugWindow);

	QObject::connect(&mainWindow, &Window::sig_notifyModelDebugWindow, &visionEngine,
			&VisionEngineWrapper::slot_modelDebugWindowClicked);

	QObject::connect(&mainWindow, &Window::sig_notifyKeyPressed, &visionEngine,
			&VisionEngineWrapper::slot_keyHandler);

	CommonRC ret = de->startEventReader();
	if (ret == CMN_RC_SUCCESS) {
		LOGMSG(LOG_DEBUG, "started reader thread");
	}

	threader.start();

	LOGMSG(LOG_DEBUG, "QT - Window show");
	mainWindow.show();

	return app.exec();;
}

