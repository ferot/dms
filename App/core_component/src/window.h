#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>

//class testcl : public QObject {
//    Q_OBJECT
//public:
//    static testcl* m_instance;
//public:
//    testcl();
//    static testcl* getInstance();
//  public slots:
//    void slotfunc();
//
//};

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    ~Window(){};
private:
    QPushButton * m_button;

signals:

public slots:
};

#endif // WINDOW_H
