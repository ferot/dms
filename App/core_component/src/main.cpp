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
#include "VisionEngine.hpp"
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

    CommunicationEngine *ce = CommunicationEngine::getInstance();(void)ce;
    DispatchEngine *de = DispatchEngine::getInstance();

    ce->connect();
    ce->subscribe("hellotopic");

    QApplication app(argc, argv);
    Threader threader;
    Window mainWindow;
    VisionEngineWrapper visionEngine;
    threader.runInThread(&visionEngine);

    QObject::connect(&mainWindow, &Window::notifyDebugWindow,
                     &visionEngine, &VisionEngineWrapper::debugWindowClicked);
    QObject::connect(&mainWindow, &Window::notifyStartTracking,
                     &visionEngine, &VisionEngineWrapper::startTracker);


    CommonRC ret = de->startEventReader();
    if (ret == CMN_RC_SUCCESS) {
        LOGMSG(LOG_DEBUG, "started reader thread");
    }

    LOGMSG(LOG_DEBUG, "QT - Window show");

    threader.start();
    mainWindow.show();

    return app.exec();;
}

