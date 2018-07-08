#include "heatmap.hpp"
#include "ModelEngine.h"
#include "StorageEngine.hpp"

//determines number of iterations which must proceed to trigger update
#define UPDATE_FREQ 50

t_array2D HeatMap::m_factorArray = t_array2D{{}};

HeatMap::HeatMap() {
    m_dimx = GRID_DIM_W;
    m_dimy = GRID_DIM_H;
    m_reloadCnt = 0;
}

HeatMap::HeatMap(std::shared_ptr<StateObject> state)  : m_state(state){
    m_dimx = state->getGridHandle()->getGridDim().first;
    m_dimy = state->getGridHandle()->getGridDim().second;
    m_reloadCnt = 0;
}

/**
 * @brief StateObject::getDatabaseCount - performs sqlite db query for :
 * -specified coords
 * -all events aggregated if those coords not provided
 * @return number of rows (events)
 */
double HeatMap::getDatabaseCount(t_p_coords coords) {
    std::shared_ptr<database> db = StorageEngine::getInstance()->getDBHandle();
    int count = 0;
    std::string statement = "select count(*) from events";

    try {
        //differentiate default val, as cannot assign std::pairf to null
        if (coords.first != -667) {
            statement += " where position = '(" + std::to_string(coords.first) + "," + std::to_string(coords.second) + ")'";
        }
        LOGMSG_ARG(LOG_TRACE, "QUERY = %s", statement.c_str());

        *db << (statement + ";") >> count;
        LOGMSG_ARG(LOG_TRACE, "NUMBER OF COUNT RETURNED = %d", count);
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
    int overallSum = getDatabaseCount(t_p_coords(-667,0)) + 1;
    LOGMSG_ARG(LOG_TRACE, "NUMBER OF EVENTS RETURNED = %d", overallSum);

    for(int i = 0; i < m_dimx; i++) {
        for(int j = 0; j < m_dimy; j++) {
            m_factorArray[i][j] = static_cast<double>(getDatabaseCount(t_p_coords(i,j))/(overallSum + 1));
        }
    }
}

/**
 * @brief HeatMap::getFactorArray - runs update to get newest values of factors
 * @return factor array to apply on image
 */
t_array2D& HeatMap::getFactorArray() {
    if (m_reloadCnt % UPDATE_FREQ == 0) {
        update();
    }
    m_reloadCnt++;

    return m_factorArray;
}
