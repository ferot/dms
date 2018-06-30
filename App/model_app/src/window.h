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
    bool m_debWinEnabled;
    bool m_modelDebWinEnabled;

    bool m_switchTracker;

    int model_win_h, model_win_w;

    QPushButton * m_button;
    QPushButton * m_buttonStartTrack;
    QPushButton * m_buttonSwitchTrack;

    keyReceiver* m_keyFilter;

    int m_grid_h_dim;
    int m_grid_w_dim;

    int m_step_x;
    int m_step_y;

    void setModelWinRes(const int& w, const int& h);
    void setGridDim(const int& w_dim, const int& h_dim);
    void updateStep();
    t_p_coords convertCoordsToGridAbstract(t_p_coords coords);

    void drawGrid(cv::Mat & image, const int&, const int&);
    void drawAdditionalInfo(cv::Mat & image, t_p_coords coords);

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
