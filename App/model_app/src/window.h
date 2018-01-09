#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>
#include <QKeyEvent>

#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>

#include "logger.h"

/**
 * Class for filtering key events.
 */
class keyReceiver : public QObject
{
    Q_OBJECT
protected:
    bool eventFilter(QObject* obj, QEvent* event);
};

/**
 * Main window class.
 */
class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);
    ~Window(){
    	cv::destroyAllWindows();
    };
private:
    bool m_debWinEnabled;
    bool m_modelDebWinEnabled;

    bool m_switchTracker;

    QPushButton * m_button;
    QPushButton * m_buttonStartTrack;
    QPushButton * m_buttonSwitchTrack;

    keyReceiver* m_keyFilter;


signals:
    void sig_notifyAppQuit();
    void sig_notifyDebugWindow(bool check);
    void sig_notifyModelWindow(bool check);
    void sig_notifySwitchTrackers(bool check);
    void sig_notifyKeyPressed(int);

public slots:
    void slot_updateModelWindow(cv::Point2d);

    void slot_modelWindowClicked(bool checked);

//    void updateTrackCoords(); //args to be sent ?
};

#endif // WINDOW_H
