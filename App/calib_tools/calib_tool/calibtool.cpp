#include "calibtool.h"
#include "ui_calibtool.h"
#include <QString>

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
    ui->label_c1_x_v->setText("aa");
//append dataset vector to file
}

void CalibTool::setValue(std::string m){
	printf("DUPA");
    ui->label_c1_x_v->setText(m.c_str());
}
