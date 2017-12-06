#include "calibtool.h"
#include "ui_calibtool.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include "logger.h"


CalibTool::CalibTool(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::CalibTool) {
    ui->setupUi(this);

    m_fann = std::make_shared<FANNWrapper>(ui);

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

    refreshValues();
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
    m_dataSet.incrCount();

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
        m_dataSet.setPayload(ui->textEdit->toPlainText());
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_9);
        out << m_dataSet;
    }
}

void CalibTool::refreshValues(){
    ui->label_datas_nr_v->setText(QString::number(m_dataSet.getCount()));
}

/**
 * @brief CalibTool::loadFromFile
 * Method responsible for opening file and deserializing it into in-memory m_dataSet member,
 * used for further manipulations.
 */
void CalibTool::loadFromFile() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open DataSet"),
            m_dataSet.getFilePath(), tr("DataSet (*.data);;All Files (*)"));

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
        in >> m_dataSet;

        refreshValues();
        if (!m_dataSet.getPayload().isEmpty()) {
            QMessageBox::information(this, tr("No data in file"),
                    tr("The dataset you are attempting to open contains no data."));
        } else {
        	ui->progressBar->setValue(0);
            ui->textEdit->setText(m_dataSet.getPayload());
            m_fann->setInputFile(fileName.toStdString());
        }
    }
}
void CalibTool::on_loadFromFileButton_clicked() {
    loadFromFile();
}

void CalibTool::on_saveToFileButton_clicked() {
    saveToFile();
}

void CalibTool::on_button_start_training_clicked()
{
	m_fann->trainNet();
}

void CalibTool::on_button_save_res_File_clicked()
{
}
