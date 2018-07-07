#ifndef STATEOBJECT_HPP
#define STATEOBJECT_HPP
#include <utility>

#include "ModelEngine.h"
#include "Grid.h"

//typedef std::pair<int,int> t_p_coords;

/**
 * @brief The StateObject class - represents state of the system (all essential infos - coords
 * heatmap, etc)
 */
class StateObject
{
private:
    std::shared_ptr<Grid> m_grid;
    t_p_coords m_coords;

public:
    StateObject(){}
    StateObject(std::shared_ptr<Grid> grid, t_p_coords coords);
    std::shared_ptr<Grid> getGridHandle() { return m_grid; }
    t_p_coords getCoords();
};

#endif // STATEOBJECT_HPP
