#ifndef CALIBTOOL_H
#define CALIBTOOL_H

#include <QMainWindow>
#include <QLabel>

#include "ui_calibtool.h"
#include "FANNWrapper.hpp"
#include "DataSet.hpp"
#include "ParamSetGenerator.hpp"

float getSpinboxFloat(QDoubleSpinBox * spinBox);
int getSpinboxInt(QSpinBox * spinBox);

namespace Ui {
class CalibTool;
}


typedef std::vector<QLabel*> t_v_qlabel;
typedef std::array<std::string, 3> t_tup_thrstrs;

class CalibTool: public QMainWindow {
	Q_OBJECT

private:

	t_v_qlabel labels_cam1;
	t_v_qlabel labels_cam2;
	t_v_qlabel labels_cam3;

	std::vector<t_v_qlabel> labels;

	DataSet m_dataSet;
    std::shared_ptr<FANNWrapper> m_fann;
    std::shared_ptr<ParamSetGenerator> m_setGenerator;

    //Used for scaling FANN inputs in <0;1> range
    float m_scaleFactorX, m_scaleFactorY;
    std::map<int,float> m_scaleMap;

    bool m_processStarted;
    bool m_processCancelled;


    void refreshValues();

	std::string formVector();
	std::string scaleInputVector(std::string, int);

	void saveToFile();
	void saveFANNDataSetRaw(DataSet&);
	void loadFromFile();

public:
	explicit CalibTool(QWidget *parent = 0);
	~CalibTool();

	void setLabelValues(t_tup_thrstrs tuple, int id);

private slots:

    void on_saveVector_clicked();

	void on_loadFromFileButton_clicked();
	void on_saveToFileButton_clicked();


    void on_button_start_training_clicked();

    void on_button_save_res_File_clicked();
    void on_button_cancel_training_clicked();

private:
	Ui::CalibTool *ui;
};

#endif // CALIBTOOL_H
