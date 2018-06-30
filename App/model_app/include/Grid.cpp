#include "Grid.h"

Grid::Grid()
{
 m_hmap = HeatMap(std::make_shared(this));
}
