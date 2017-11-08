#include "window.h"

Window::Window(QWidget *parent) : QWidget(parent)
{
    setFixedSize(640,480);
    m_button = new QPushButton("Debug Window", this);
    m_button->setGeometry(10, 10, 150, 30);
    m_button->setToolTip("Toggle on/off videostream preview");

    connect(m_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));
}
