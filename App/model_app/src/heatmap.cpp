#include "heatmap.hpp"
#include "ModelEngine.h"
#include "StorageEngine.hpp"

HeatMap::HeatMap(std::shared_ptr<StateObject> state)  : m_state(state){
//    m_dimx = state->dimX;
//    m_dimy = state->dimY;
}

//HeatMap::HeatMap(){
////    m_dimx = state->dimX;
////    m_dimy = state->dimY;
//}


/**
 * @brief StateObject::getDatabaseCount - performs sqlite db query for :
 * -specified coords
 * -all events aggregated if those coords not provided
 * @return number of rows (events)
 */
int HeatMap::getDatabaseCount(t_p_coords coords) {
    std::shared_ptr<database> db = StorageEngine::getInstance()->getDBHandle();
    int count = 0;
    std::string statement = "select count(*) from events";

    try {
        //differentiate default val, as cannot assign std::pairf to null
        if (coords.first == -667) {
            statement += " where position = '(" + std::to_string(coords.first) + "," + std::to_string(coords.second) + ")'";
        }
        *db << statement >> count;

    } catch (exception& e) {
        LOGMSG_ARG(LOG_ERROR,
                   "Exception %s when trying to perform statement !",
                   e.what());
    }
    return count;
}

/**
 * @brief HeatMap::update - updates ratio factors for heatmap
 * it's simply count on each (x,y) position divided to sum of all counts
 * on all positions
 */
void HeatMap::update() {
    //TODO : get data from database (select + count)
    // generate array for applying factors

    int overallSum = getDatabaseCount();
    for(int i = 0; i < m_dimx; i++) {
        for(int j = 0; i < m_dimy; i++) {
            m_factorArray[i][j] = getDatabaseCount(t_p_coords(i,j))/(overallSum + 1);
        }
    }
}
