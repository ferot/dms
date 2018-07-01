#include "Grid.h"
#include "heatmap.hpp"

Grid::Grid(cv::Mat & image, int dim_w, int dim_h)
{
    m_image = image;
    m_hmap = std::make_shared<HeatMap>();

    model_win_h = 600;
    setGridDim(dim_w, dim_h);
}

cv::Mat& Grid::getImage() {
    return m_image;
}

cv::Mat& Grid::draw(){
    drawHeatMap();
    drawGrid(m_step_x, m_step_y);
    drawPoint();

    return m_image;
}

/**
 * Draws grid lines and additional info.
 * @param step_x
 * @param step_y
 * @return image with drawn grid.
 */
void Grid::drawGrid(const int& step_x, const int& step_y){
    int thickness = 2;
    int lineType = cv::LINE_8;

    for(int x = 0; x<m_grid_w_dim; x++){
        for(int y=0; y<m_grid_h_dim; y++){
            //vertical
            cv::line(m_image, cv::Point(step_x*x, 0), cv::Point(step_x*x, model_win_h),
                     cv::Scalar(0, 0, 0), thickness, lineType);
            //horizontal
            cv::line(m_image, cv::Point(0, step_y*y), cv::Point(model_win_h, step_y*y),
                     cv::Scalar(0, 0, 0), thickness, lineType);
        }
    }
}

t_p_coords Grid::convertCoordsToGridAbstract(t_p_coords coords){
    int result_x = m_step_x*coords.first + m_step_x/2;
    int result_y = m_step_y*coords.second + m_step_y/2;

    return t_p_coords(result_x, result_y);
}

void Grid::drawHeatMap() {

}

void Grid::drawPoint() {
    cv::circle(m_image, cv::Point(m_resultPoint.first, m_resultPoint.second), 5, cv::Scalar(0, 0, 255), cv::FILLED, cv::LINE_4);
}

/**
 * Sets position grid dimensions : (w_dim x h_dim).
 * @param w_dim
 * @param h_dim
 */
void Grid::setGridDim(const int w_dim, const int h_dim){
    m_grid_h_dim = h_dim;
    m_grid_w_dim = w_dim;

    //as grid dim changed, update step to properly draw grid
    updateStep();
}

/**
 * Refreshes step value for properly drawing grid lines and resultPoint.
 */
void Grid::updateStep(){
    m_step_x = model_win_h/m_grid_h_dim;
    m_step_y = model_win_h/m_grid_w_dim;
}

t_p_coords Grid::getGridDim() {
    return t_p_coords(m_grid_h_dim, m_grid_w_dim);
}

void Grid::setPointCoords(t_p_coords coords){
    m_resultPoint = coords;
}

