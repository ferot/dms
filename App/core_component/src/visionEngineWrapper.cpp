#include "visionEngineWrapper.h"

VisionEngineWrapper::VisionEngineWrapper(QObject *parent) : QObject(parent)
{
    m_visionEngine = VisionEngine::getInstance();
    m_visionEngine->addTracker("KCF", 0);
}

void VisionEngineWrapper::startTracker(){

    m_visionEngine->startAllTrackers();
}

void VisionEngineWrapper::stopTracker(){
    m_visionEngine->stopAllTrackers();
}

void VisionEngineWrapper::debugWindowClicked(bool switched){
    m_visionEngine->displayDebugWindow();
}
