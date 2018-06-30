#include "stateobject.hpp"

StateObject::StateObject(std::shared_ptr<Grid> grid, t_p_coords coords) : m_grid(grid), m_coords(coords)
{
    m_grid->setPointCoords(m_coords);
}

t_p_coords StateObject::getCoords() {
    return m_coords;
}
