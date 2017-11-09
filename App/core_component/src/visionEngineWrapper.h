#ifndef VISIONENGINEWRAPPER_H
#define VISIONENGINEWRAPPER_H

#include <QObject>

#include "VisionEngine.hpp"

class VisionEngineWrapper : public QObject
{
    Q_OBJECT
private:
    VisionEngine * m_visionEngine;
public:
    explicit VisionEngineWrapper(QObject *parent = nullptr);
signals:
    void notifyDebugWindow();
public slots:
    void debugWindowClicked(bool);
    void startTracker();
    void stopTracker();
};

#endif // VISIONENGINEWRAPPER_H
