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
    QPushButton * m_button;
    QPushButton * m_buttonStartTrack;

    keyReceiver* m_keyFilter;


signals:
    void sig_notifyAppQuit();
    void sig_notifyDebugWindow(bool check);
    void sig_notifyStartTracking(bool check);
    void sig_notifyKeyPressed(int);

public slots:
    void slot_updateDebugWindow(cv::Mat frame);

    void slot_debugWindowClicked(bool checked);
    void slot_startTracking(bool checked);

//    void updateTrackCoords(); //args to be sent ?
};

#endif // WINDOW_H
