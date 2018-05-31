#ifndef CALIBTOOL_H
#define CALIBTOOL_H

#include <QMainWindow>
#include <QLabel>

#include "Event.hpp"
#include "DispatchEngine.hpp"
#include "ui_calibtool.h"
#include "FANNWrapper.hpp"
#include "DataSet.hpp"
#include "ParamSetGenerator.hpp"

float getSpinboxFloat(QDoubleSpinBox * spinBox);
int getSpinboxInt(QSpinBox * spinBox);

namespace Ui {
class CalibTool;
}

struct TrainJob {
private:
	std::shared_ptr<FANNWrapper> m_fann;
	std::thread m_thrHandle;
	int id;

public:

	void setThrHandle(std::thread thr) {
		m_thrHandle = std::move(thr);
	}

	void run() {
		m_fann->trainNet();
		t_eventPtr event = std::make_shared<Event>(
				eventType::JOB_FINISHED_EVENT, std::to_string(id));

		DispatchEngine::getInstance()->enqueueEvent(event);
	}

	TrainJob(std::shared_ptr<FANNWrapper> ptr, int _id) :
			m_fann(std::move(ptr)), id(_id) {
	}

	~TrainJob() {
		m_thrHandle.join();
	}
};

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

    std::map<int, std::shared_ptr<TrainJob>> m_jobs;

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

	void scheduleJobs();


public:
	explicit CalibTool(QWidget *parent = 0);
	~CalibTool();

	void setProgressBar(int val);
	void setLabelValues(t_tup_thrstrs tuple, int id);
	void removeJob(int id);

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
