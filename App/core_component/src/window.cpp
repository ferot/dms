#include "window.h"

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    setFixedSize(640,480);

    //debug window
    m_button = new QPushButton("Debug Window", this);
    m_button->setGeometry(10, 10, 150, 30);
    m_button->setToolTip("Toggle on/off videostream preview");
    m_button->setCheckable(true);

    m_buttonStartTrack = new QPushButton("StartTracker", this);
    m_buttonStartTrack->setGeometry(10, 50, 150, 30);
    m_buttonStartTrack->setToolTip("Starts tracking");
    m_buttonStartTrack->setCheckable(true);


    connect(m_button, SIGNAL (clicked(bool)),
            this, SLOT (slotDebugWindowClicked(bool)));

    connect(m_buttonStartTrack, SIGNAL (clicked(bool)),
            this, SLOT (slotStartTracking(bool)));

}

void Window::slotDebugWindowClicked(bool checked){
    emit notifyDebugWindow(checked);
}

void Window::slotStartTracking(bool checked){
    emit notifyStartTracking(checked);
}
