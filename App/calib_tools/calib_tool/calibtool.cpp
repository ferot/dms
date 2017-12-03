#include "calibtool.h"
#include "ui_calibtool.h"
#include <QString>

CalibTool::CalibTool(QWidget *parent) :
		QMainWindow(parent), ui(new Ui::CalibTool) {
	ui->setupUi(this);

	labels_cam1 = { {ui->label_c1_x_v}, {ui->label_c1_y_v}, {ui->label_c1_s_v}};
	labels_cam2 = {ui->label_c2_x_v, ui->label_c2_y_v, ui->label_c2_s_v};
	labels_cam3 = {ui->label_c3_x_v, ui->label_c3_y_v, ui->label_c3_s_v};

	labels = {labels_cam1, labels_cam2, labels_cam3};

}

CalibTool::~CalibTool() {
	delete ui;
}

void CalibTool::on_pushButton_clicked() {
//append dataset vector to file
}

void CalibTool::setLabelValues(t_tup_thrstrs tuple, int id) {
	auto &lab = labels[id];
	int index = 0;
	std::for_each(lab.begin(), lab.end(), [&](QLabel* label) {
		label->setText(tuple[index++].c_str());
	});
}
