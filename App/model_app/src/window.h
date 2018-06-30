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
#include "ModelEngine.h"
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
    cv::Mat m_imageBuffer;
    bool m_debWinEnabled;
    bool m_modelDebWinEnabled;

    bool m_switchTracker;

    int model_win_h, model_win_w;

    QPushButton * m_button;
    QPushButton * m_buttonStartTrack;
    QPushButton * m_buttonSwitchTrack;

    keyReceiver* m_keyFilter;

    void setModelWinRes(const int& w, const int& h);
    void drawAdditionalInfo(cv::Mat & image, StateObject state);

signals:
    void sig_notifyAppQuit();
    void sig_notifyDebugWindow(bool check);
    void sig_notifyModelWindow(bool check);
    void sig_notifySwitchTrackers(bool check);
    void sig_notifyKeyPressed(int);

public slots:
    void slot_updateModelWindow(StateObject state);

    void slot_modelWindowClicked(bool checked);

//    void updateTrackCoords(); //args to be sent ?
};

#endif // WINDOW_H
