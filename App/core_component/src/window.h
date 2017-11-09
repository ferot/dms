#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>

class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    ~Window(){};
private:
    QPushButton * m_button;
    QPushButton * m_buttonStartTrack;

signals:
    void notifyAppQuit();
    void notifyDebugWindow(bool check);
    void notifyStartTracking(bool check);
public slots:
    void slotDebugWindowClicked(bool checked);
    void slotStartTracking(bool checked);
//    void updateTrackCoords(); //args to be sent ?
};

#endif // WINDOW_H
