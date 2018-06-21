#ifndef ModelEngine_H
#define ModelEngine_H

#include <QObject>
#include <QTimer>

//#include "floatfann.h"
//#include "fann_cpp.h"
#include "fann.h"

#include "CommunicationEngine.hpp"
#include "CommEvent.hpp"
#include "Config.hpp"
#include "DispatchEngine.hpp"
#include "VisionEngine.hpp"
#include "StorageEngine.hpp"

typedef std::array<std::string, 3> t_tup_thrstrs;
typedef struct fann t_fann_s;
typedef std::shared_ptr<fann_type> t_ptr_fann_type;

const int cam_nrs = 3;

enum State {
	INIT_S, FIND_TRGT_S, INIT_TRCK_S, RUN_TRCK_S, VERIF_TRGT_S, STOP_S = 0xff
};

/**
 * IDs for mapping scale factors for input from cameras
 */
enum scaleID{
	X = 0,//!< X
	Y,    //!< Y
	SIZE  //!< SIZE
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
            cam_coords[id]= tuple;
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
	StorageEngine *se;

	Coords m_coords;
	t_fann_s * m_ann;

	bool m_modelWinEnabled;

	//Used for scaling FANN inputs in <0;1> range
    float m_scaleFactorX, m_scaleFactorY;
    std::map<int,float> m_scaleMap;

	void worker();
	void performDatabaseStatement(int x, int y, int id = 0);

public:

	explicit ModelEngine();
	~ModelEngine();

	CommonRC loadNetFile(std::string filepath);
    fann_type* calculateResult(std::array<float,9> input);
    std::array<float,9> obtainInputVec();

	const t_tup_thrstrs& getCoords(int id);
	void setCoords(int id, t_tup_thrstrs tuple);
	void printCamDebug();

	signals:
	void sig_notifyModelWindow(int x, int y);

	public slots:
	void slot_modelWindowButtonClicked(bool);

};

#endif // ModelEngine_H
