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
    explicit Window(int camID, QWidget *parent = nullptr);
    ~Window(){
    	cv::destroyAllWindows();
    };
private:
    bool m_debWinEnabled;

    bool m_switchTracker;

    QPushButton * m_button;
    QPushButton * m_buttonStartTrack;
    QPushButton * m_buttonSwitchTrack;

    keyReceiver* m_keyFilter;

    void createWindows(int camID);

signals:
    void sig_notifyAppQuit();
    void sig_notifyDebugWindow(bool check);
    void sig_notifySwitchTrackers(bool check);
    void sig_notifyKeyPressed(int);

public slots:
    void slot_updateDebugWindow(cv::Mat frame);
    void slot_debugWindowClicked(bool checked);
};

#endif // WINDOW_H
