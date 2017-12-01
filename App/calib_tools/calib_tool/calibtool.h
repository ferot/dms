#ifndef CALIBTOOL_H
#define CALIBTOOL_H

#include <QMainWindow>

namespace Ui {
class CalibTool;
}

class CalibTool : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibTool(QWidget *parent = 0);
    ~CalibTool();

    void setValue(std::string m);
private slots:

    void on_pushButton_clicked();

private:
    Ui::CalibTool *ui;
};

#endif // CALIBTOOL_H
