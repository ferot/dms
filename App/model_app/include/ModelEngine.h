#ifndef ModelEngine_H
#define ModelEngine_H

#include <QObject>
#include <QTimer>

#include "floatfann.h"
#include "fann_cpp.h"

#include "CommunicationEngine.hpp"
#include "CommEvent.hpp"
#include "Config.hpp"
#include "DispatchEngine.hpp"
#include "VisionEngine.hpp"

typedef std::array<std::string, 3> t_tup_thrstrs;
typedef struct fann t_fann_s;

const int cam_nrs = 3;

enum State {
	INIT_S, FIND_TRGT_S, INIT_TRCK_S, RUN_TRCK_S, VERIF_TRGT_S, STOP_S = 0xff
};

/**
 * Struct representing both real and camera-abstract coords
 */
struct Coords{
	std::vector<t_tup_thrstrs> cam_coords;
	int real_x;
	int real_y;

	Coords(): real_x(0), real_y(0){
		cam_coords.reserve(cam_nrs);

		t_tup_thrstrs zeroTuple = {"0","0","0"};
		for(int i = 0; i < cam_nrs; i++){
			cam_coords.push_back(zeroTuple);
		}
	}

	void setRow(t_tup_thrstrs tuple, int id){
		for(int i = 0; i<cam_nrs; i++){
			cam_coords[id][i] = tuple[i];
		}
	}

	const t_tup_thrstrs& getRow(int id){

			return cam_coords[id];
	}

};


class ModelEngine: public QObject {
	Q_OBJECT
private:

	QTimer rythm;
	CommunicationEngine *ce;
	DispatchEngine *de;

	Coords m_coords;
	t_fann_s * m_ann;

	bool m_modelWinEnabled;

	void worker();

public:

	explicit ModelEngine();
	~ModelEngine();

	CommonRC loadNetFile(std::string filepath);
	fann_type* calculateResult(fann_type input);

	const t_tup_thrstrs& getCoords(int id);
	void setCoords(int id, t_tup_thrstrs tuple);
	void printCamDebug();

	signals:
	void sig_notifyModelWindow(int x, int y);

	public slots:
	void slot_modelWindowButtonClicked(bool);

};

#endif // ModelEngine_H
