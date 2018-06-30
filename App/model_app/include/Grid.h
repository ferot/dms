#ifndef GRID_H
#define GRID_H
#include "heatmap.hpp"

class Grid
{
private:
    void drawPoint();
    void drawHeatMap();
    HeatMap m_hmap;
public:
    Grid();
    void drawGrid();
    cv::Mat getImage();

};

#endif // GRID_H
