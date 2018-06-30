#ifndef STATEOBJECT_HPP
#define STATEOBJECT_HPP
#include <utility>

#include "ModelEngine.h"
#include "Grid.h"

typedef std::pair<int,int> t_p_coords;

/**
 * @brief The StateObject class - represents state of the system (all essential infos, heatmap, etc)
 */
class StateObject
{
private:
    t_p_coords m_coords;
    std::shared_ptr<Grid> m_grid;
public:
    StateObject(t_p_coords coords);
    t_p_coords getCoords();
};

#endif // STATEOBJECT_HPP
