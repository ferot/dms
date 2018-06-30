#include "stateobject.hpp"

StateObject::StateObject(t_p_coords coords) : m_coords(coords)
{

}

t_p_coords StateObject::getCoords() {
    return m_coords;
}
