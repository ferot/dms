#ifndef CALIBTOOL_H
#define CALIBTOOL_H

#include <future>

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
	int id;

public:
    void run() {
        LOGMSG_ARG(LOG_DEBUG, "EXECUTING THR WITH ID = %d", id);
        m_fann->trainNet();
        t_eventPtr event = std::make_shared<Event>(
                eventType::JOB_FINISHED_EVENT, std::to_string(id));

        DispatchEngine::getInstance()->enqueueEvent(event);
    }

	TrainJob(std::shared_ptr<FANNWrapper> ptr, int _id) :
			m_fann((ptr)), id(_id) {
	}

	~TrainJob() {
	}
};

typedef std::vector<QLabel*> t_v_qlabel;
typedef std::array<std::string, 3> t_tup_thrstrs;
typedef std::map<int, std::shared_ptr<TrainJob>> t_map_idJob;

class CalibTool: public QMainWindow {
	Q_OBJECT

private:

	t_v_qlabel labels_cam1;
	t_v_qlabel labels_cam2;
	t_v_qlabel labels_cam3;

	std::vector<t_v_qlabel> labels;

    std::string m_netDir;
    std::string m_dataSetDir;
	DataSet m_dataSet;
    std::shared_ptr<FANNWrapper> m_fann;
    std::shared_ptr<ParamSetGenerator> m_setGenerator;

    t_map_idJob m_jobs;

    //Used for scaling FANN inputs in <0;1> range
    float m_scaleFactorX, m_scaleFactorY;
    std::map<int,float> m_scaleMap;

    bool m_processStarted;
    bool m_processCancelled;

    int m_jobCount;

    void refreshValues();

	std::string formVector();
	std::string scaleInputVector(std::string, int);

	void saveToFile();
	void saveFANNDataSetRaw(DataSet&);
	void loadFromFile();

	void scheduleJobs();


public:
    std::mutex m_jobMutex;

	explicit CalibTool(QWidget *parent = 0);
	~CalibTool();

	void setProgressBar(int val);
    void setJobCountLabel(int val);
	void setLabelValues(t_tup_thrstrs tuple, int id);
	void removeJob(int id);
    void notifyProcessedJob(int id);

	t_map_idJob getJobMap();

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
