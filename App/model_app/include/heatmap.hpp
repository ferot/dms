#ifndef HEATMAP_HPP
#define HEATMAP_HPP
#include "stateobject.hpp"
#include "ModelEngine.h"

typedef std::array<std::array<int, 9>,9> t_array2D;
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
public:
    HeatMap(){}

    HeatMap(std::shared_ptr<StateObject>state);
    void update();

};

#endif // HEATMAP_HPP
