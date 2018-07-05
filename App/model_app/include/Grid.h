#ifndef GRID_H
#define GRID_H
#include <utility>
#include "heatmap.hpp"

typedef std::pair<int,int> t_p_coords;
class HeatMap;
class Grid
{
private:
    shared_ptr<HeatMap> m_hmap;
    cv::Mat m_image;
    int m_grid_h_dim;
    int m_grid_w_dim;

    int model_win_h;

    int m_step_x;
    int m_step_y;

    t_p_coords m_resultPoint;

    void drawPoint();
    void drawHeatMap();

    t_p_coords convertCoordsToGridAbstract(t_p_coords coords);
    void applyFactors();
    cv::Scalar mapFreqToFactor(float freq);

    void updateStep();

public:

    Grid(cv::Mat & image, int dim_w, int dim_h);
    cv::Mat& draw();
    void setPointCoords(t_p_coords coords);
    void setGridDim(const int w_dim, const int h_dim);
    void drawGrid(const int&, const int&);
    cv::Mat &getImage();

    t_p_coords getGridDim();
};

#endif // GRID_H
