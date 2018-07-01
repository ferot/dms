#ifndef HEATMAP_HPP
#define HEATMAP_HPP
#include "stateobject.hpp"
#include "ModelEngine.h"

//GRID dimensions
#define GRID_DIM_W 5
#define GRID_DIM_H 5

typedef std::array<std::array<float, GRID_DIM_W>,GRID_DIM_H> t_array2D;
typedef std::pair<int,int> t_p_coords;

class StateObject;
/**
 * @brief The HeatMap class responsible for depicting 'condensation' of object in area
 */
class HeatMap {
private:
    t_array2D m_factorArray;
    std::shared_ptr<StateObject> m_state;
    int m_dimy, m_dimx;

    int getDatabaseCount(t_p_coords coords = t_p_coords(-667,-667));
    void update();

public:
    HeatMap(){}

    HeatMap(std::shared_ptr<StateObject>state);
    t_array2D getFactorArray();

};

#endif // HEATMAP_HPP
