#ifndef CALIBTOOL_H
#define CALIBTOOL_H

#include <QMainWindow>
#include <QLabel>
#include "FANNWrapper.hpp"

#include "DataSet.hpp"

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

    void refreshValues();

	std::string formVector();
	void saveToFile();
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

private:
	Ui::CalibTool *ui;
};

#endif // CALIBTOOL_H
