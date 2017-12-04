#include "calibtool.h"
#include "ui_calibtool.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

CalibTool::CalibTool(QWidget *parent) :
		QMainWindow(parent), m_trainingSetCount(0), ui(new Ui::CalibTool) {
	ui->setupUi(this);

	labels_cam1 = { {ui->label_c1_x_v}, {ui->label_c1_y_v}, {ui->label_c1_s_v}};
	labels_cam2 = {ui->label_c2_x_v, ui->label_c2_y_v, ui->label_c2_s_v};
	labels_cam3 = {ui->label_c3_x_v, ui->label_c3_y_v, ui->label_c3_s_v};

	labels = {labels_cam1, labels_cam2, labels_cam3};

}

CalibTool::~CalibTool() {
	delete ui;
}

void CalibTool::on_saveVector_clicked() {
//append dataset vector to file
	QString buffer = ui->textEdit->toPlainText()
			+ QString::fromStdString(formVector());

	ui->textEdit->setText(buffer);
}

void CalibTool::setLabelValues(t_tup_thrstrs tuple, int id) {
	auto &lab = labels[id];
	int index = 0;

	std::for_each(lab.begin(), lab.end(), [&](QLabel* label) {
		label->setText(tuple[index++].c_str());
	});
}

/**
 * @brief CalibTool::formVector
 * Method responsible for aggregating actual labels values (obtained from cameras),
 * and forming string vector, which will be put in FANN training set.
 * @return
 */
std::string CalibTool::formVector() {
	std::string resultVector;

	for (auto lab : labels) {
		std::for_each(lab.begin(), lab.end(), [&](QLabel* label) {
			resultVector.append(label->text().toStdString()+ " ");
		});
	}

	resultVector.append(
			"\n" + ui->spinBox->text().toStdString() + " "
					+ ui->spinBox_2->text().toStdString() + "\n");
	m_trainingSetCount++;

	return resultVector;
}

void CalibTool::saveToFile() {
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save DataSet"),
			"", tr("DataSet (*.data);;All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
					file.errorString());
			return;
		}

		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_4_9);
		out << ui->textEdit->toPlainText();
	}
}

void CalibTool::loadFromFile() {
	QString textEditBuffer;
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open DataSet"),
			"", tr("DataSet (*.data);;All Files (*)"));

	if (fileName.isEmpty())
		return;
	else {

		QFile file(fileName);

		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
					file.errorString());
			return;
		}

		QDataStream in(&file);
		in.setVersion(QDataStream::Qt_4_9);
		ui->textEdit->clear();
		in >> textEditBuffer;

		if (textEditBuffer.isEmpty()) {
			QMessageBox::information(this, tr("No data in file"),
					tr(
							"The dataset you are attempting to open contains no data."));
		} else {
			ui->textEdit->setText(textEditBuffer);
		}
	}
}
void CalibTool::on_loadFromFileButton_clicked() {
	loadFromFile();
}

void CalibTool::on_saveToFileButton_clicked() {
	saveToFile();
}
