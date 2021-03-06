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
#include "stateobject.hpp"
#include "Grid.h"

#define FEATURE_COUNT 3

// describes total number of features used (determined by number of cams and features they provide - default: 3)
#define TOTAL_INPUT_COUNT (cam_nrs*FEATURE_COUNT)

const int cam_nrs = 4;

class StateObject;
class Grid;

typedef std::array<std::string, 3> t_tup_thrstrs;
typedef struct fann t_fann_s;
typedef std::shared_ptr<fann_type> t_ptr_fann_type;
typedef std::pair<int,int> t_p_coords;
typedef std::shared_ptr<Grid> t_ptr_grid;
typedef std::array<float, TOTAL_INPUT_COUNT> t_input_array;

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
    static ModelEngine* m_instance;

	QTimer rythm;
	CommunicationEngine *ce;
	DispatchEngine *de;
	StorageEngine *se;

    t_ptr_grid m_grid;
    cv::Mat m_imageBuffer;

    Coords m_coords;

	t_fann_s * m_ann;
	bool m_modelWinEnabled;

	//Used for scaling FANN inputs in <0;1> range
    float m_scaleFactorX, m_scaleFactorY;
    std::map<int,float> m_scaleMap;

	void worker();
	void performDatabaseStatement(int x, int y, int id = 0);

public:
    ModelEngine* getInstance();

	explicit ModelEngine();
	~ModelEngine();

	CommonRC loadNetFile(std::string filepath);
    fann_type* calculateResult(t_input_array m_input);
    t_input_array obtainInputVec();

	const t_tup_thrstrs& getCoords(int id);
	void setCoords(int id, t_tup_thrstrs tuple);
	void printCamDebug();

	signals:
    void sig_notifyModelWindow(StateObject state);

	public slots:
	void slot_modelWindowButtonClicked(bool);

};

#endif // ModelEngine_H
