#ifndef STATEOBJECT_HPP
#define STATEOBJECT_HPP
#include <utility>

typedef std::pair<int,int> t_p_coords;

/**
 * @brief The StateObject class - represents state of the system (all essential infos, heatmap, etc)
 */
class StateObject
{
private:
    t_p_coords m_coords;
public:
    StateObject(t_p_coords coords);
    t_p_coords getCoords();
};

#endif // STATEOBJECT_HPP
