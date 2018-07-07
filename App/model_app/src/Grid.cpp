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
    //inversion of coords is essential to keep table-indexing !
    int result_x = m_step_x*coords.second + m_step_x/2;
    int result_y = m_step_y*coords.first + m_step_y/2;

    return t_p_coords(result_x, result_y);
}

void Grid::drawHeatMap() {
    LOGMSG(LOG_TRACE, "Drawing heatmap");
    applyFactors();
}

void Grid::drawPoint() {
    cv::circle(m_image, cv::Point(m_resultPoint.first, m_resultPoint.second), 8, cv::Scalar(0, 0, 0), cv::FILLED, cv::LINE_8);
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

/**
 * @brief mapFreqToFactor - creates color value for grid based on frequency
 * @param freq - counted value of object appearance
 * @return - cv::Scalar : 3-channel BGR value
 */
cv::Scalar Grid::mapFreqToFactor(float freq) {
    if(freq >= 0 && freq < 0.2) {
        return cv::Scalar(static_cast<int>(255*(1-freq)), 0, 0); //COLD BLUE
    } else if(freq >= 0.2 && freq < 0.4) {
        return cv::Scalar(static_cast<int>(255*freq), static_cast<int>(255*freq), 0); //WARMER BLUE-GREEN
    } else if(freq >= 0.4 && freq < 0.6) {
        return cv::Scalar(0, static_cast<int>(255*freq), static_cast<int>(255*freq)); //GREENISH
    } else if (freq >= 0.6 && freq < 0.8) {
        return cv::Scalar(0, static_cast<int>(255*(1-freq)), static_cast<int>(255*freq)); //WARMER RED
    } else {
        return cv::Scalar(0, 0, 255); //RED
    }
}

/**
 * @brief Grid::applyFactors - based on counted factors of object appearance frequency,
 * method generates color blocks and applies them on image to depict heatmap
 */
void Grid::applyFactors() {
    auto& arr = m_hmap->getFactorArray();

    //Assumption for safety - grid has same dimensions!
    cv::Size s = m_image.size();
    int rows = s.height;
    int cols = s.width;

    //steps for grid
    int w_offset = rows/GRID_DIM_W;
    int h_offset = cols/GRID_DIM_H;

    //temp values of grid position (bsed on current w_offset iteration)
    int x_off = 0;
    int y_off = 0;

    //iterate over abstract grid
    for(int x = 0; x < GRID_DIM_W; x++) {
        for (int y = 0; y < GRID_DIM_H; y++, x_off += w_offset){
            cv::Mat block(cv::Size(w_offset, h_offset), CV_8UC3, mapFreqToFactor(arr[x][y]));
            block.copyTo(m_image(cv::Rect2d(x_off, y_off, block.rows, block.cols)));
        }
        x_off = 0;
        y_off += h_offset;

    }
}

/**
 * @brief Grid::setState - uses state param to update current point position and other features.
 * @param obj
 */
void Grid::setState(StateObject obj) {
    setPointCoords(convertCoordsToGridAbstract(obj.getCoords()));
}
