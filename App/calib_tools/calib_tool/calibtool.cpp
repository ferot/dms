#include "calibtool.h"
#include "ui_calibtool.h"

CalibTool::CalibTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CalibTool)
{
    ui->setupUi(this);
}

CalibTool::~CalibTool()
{
    delete ui;
}

void CalibTool::on_pushButton_clicked()
{
//append dataset vector to file
}
