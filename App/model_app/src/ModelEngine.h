#ifndef ModelEngine_H
#define ModelEngine_H

#include <QObject>
#include <QTimer>

#include "VisionEngine.hpp"

enum State {
	INIT_S, FIND_TRGT_S, INIT_TRCK_S, RUN_TRCK_S, VERIF_TRGT_S, STOP_S = 0xff
};

class ModelEngine: public QObject {
	Q_OBJECT
private:
	QTimer rythm;
	bool m_modelWinEnabled;

	void worker();

public:
	explicit ModelEngine();
	~ModelEngine();

	signals:

	void sig_notifyModelWindow(cv::Point);

	public slots:

	void slot_modelWindowButtonClicked(bool);

};

#endif // ModelEngine_H
