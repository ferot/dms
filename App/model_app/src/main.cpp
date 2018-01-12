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



#include <iostream>
#include <thread>
#include <QApplication>
#include <QPushButton>
#include <QObject>
#include "window.h"
#include "Event.hpp"
#include "threader.h"

#include "ModelEngine.h"

using namespace std;

int main(int argc, char **argv) {

	if (!initLogger()) {
		LOGMSG(LOG_DEBUG, "Logger initialized!");
	} else {
		printf("Logger init failed");
	}

	LOGMSG(LOG_DEBUG, "Starting core_app main!");


	QApplication app(argc, argv);

	Threader threader;
	Window mainWindow;
	ModelEngine me;

	threader.runInThread(&me);

	qRegisterMetaType<cv::Mat>("cv::Mat");
	qRegisterMetaType<cv::Point>("cv::Point");

	QObject::connect(&mainWindow, &Window::sig_notifyModelWindow, &me,
			&ModelEngine::slot_modelWindowButtonClicked);

	QObject::connect(&me, &ModelEngine::sig_notifyModelWindow,
			&mainWindow, &Window::slot_updateModelWindow);

	threader.start();

	LOGMSG(LOG_DEBUG, "QT - Window show");
	mainWindow.show();

	return app.exec();;
}

