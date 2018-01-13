#include "window.h"
#include "Common.hpp"
/**
 * Constructor of app's window.
 * Sets up all widgets and connects appropriate internal signals/slots
 * @param parent
 */
Window::Window(QWidget *parent) :
		QMainWindow(parent) {
	setFixedSize(320, 320);

	m_keyFilter = new keyReceiver();
	this->installEventFilter(m_keyFilter);

	m_debWinEnabled = false;
	m_modelDebWinEnabled = false;

	setModelWinRes(600,600);
	setGridDim(5,5);

	m_buttonStartTrack = new QPushButton("model_window", this);
	m_buttonStartTrack->setGeometry(10, 50, 150, 30);
	m_buttonStartTrack->setToolTip("Starts tracking");
	m_buttonStartTrack->setCheckable(true);

	connect(m_buttonStartTrack, SIGNAL(clicked(bool)), this,
			SLOT(slot_modelWindowClicked(bool)));

}

/*** SLOTS ***/


/**
 * Slot emitting signal to start tracking procedure.
 * @param checked
 */
void Window::slot_modelWindowClicked(bool checked) {
	m_modelDebWinEnabled = (checked == true) ? true : false;
	emit sig_notifyModelWindow(checked);
}

/**
 * Updates view of objects position on debug window by input from ModelEngine.
 * @param x - real x coord
 * @param y - real y coord
 */
void Window::slot_updateModelWindow(int x, int y) {

	int result_x = m_step_x*x + m_step_x/2;
	int result_y = m_step_y*y + m_step_y/2;

	//inversion of coords is essential to keep table-indexing
	cv::Point resultPoint(result_y, result_x);

	cv::Mat image = drawGrid(m_step_x, m_step_y);
	cv::circle(image, resultPoint, 5, cv::Scalar(0, 0, 255), cv::FILLED, cv::LINE_4);

	if (m_modelDebWinEnabled) {
		cv::imshow("MODEL_DEBUG", image);

		cv::moveWindow("MODEL_DEBUG", 10, 500);

	} else {
		cv::destroyWindow("MODEL_DEBUG");
	}

}

/**
 * Creates image and draws grid lines and additional info.
 * @param step_x
 * @param step_y
 * @return image with drawn grid.
 */
cv::Mat Window::drawGrid(const int& step_x, const int& step_y){
	cv::Mat image = cv::Mat(model_win_h, model_win_h, CV_8UC3, cv::Scalar(200, 200, 200));
	int thickness = 2;
	int lineType = cv::LINE_8;

	for(int x = 0; x<m_grid_w_dim; x++){
		for(int y=0; y<m_grid_h_dim; y++){
			//vertical
			cv::line(image, cv::Point(step_x*x, 0), cv::Point(step_x*x, model_win_h),
					cv::Scalar(0, 0, 0), thickness, lineType);
			//horizontal
			cv::line(image, cv::Point(0, step_y*y), cv::Point(model_win_h, step_y*y),
					cv::Scalar(0, 0, 0), thickness, lineType);
		}
	}

	cv::putText(image, "(0,0)", cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5,
			cv::Scalar(50, 170, 50), 2);

	std::string endCoords("(" + numToString(m_grid_w_dim-1) + "," + numToString(m_grid_h_dim-1)+ ")");
	cv::putText(image, endCoords, cv::Point(model_win_h - 50, model_win_h - 10), cv::FONT_HERSHEY_SIMPLEX,
			0.5, cv::Scalar(50, 170, 50), 2);

	return image;
}

/**
 * Sets position grid dimensions : (w_dim x h_dim).
 * @param w_dim
 * @param h_dim
 */
void Window::setGridDim(const int& w_dim, const int& h_dim){
	m_grid_h_dim = h_dim;
	m_grid_w_dim = w_dim;

	//as grid dim changed, update step to properly draw grid
	updateStep();
}

/**
 * Refreshes step value for properly drawing grid lines and resultPoint.
 */
void Window::updateStep(){
	m_step_x = model_win_h/m_grid_h_dim;
	m_step_y = model_win_h/m_grid_w_dim;
}

/**
 * Sets dimension of debug window.
 * @param w
 * @param h
 */
void Window::setModelWinRes(const int& w, const int& h){
	model_win_h = h;
	model_win_w = w;
}

/**
 * Method filtering key press action and sending appropriate signal.
 * If event is not classified as key, it's resent for further processing
 * @param obj - pointer to object which received event.
 * @param event - specific event to be filtered
 * @return true if event is from key class, else false.
 */
bool keyReceiver::eventFilter(QObject* obj, QEvent* event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent* key = static_cast<QKeyEvent*>(event);
		int keyCode = key->key();

		Window * win = static_cast<Window*>(obj);
		LOGMSG_ARG(LOG_TRACE,
				"[keyReceiver::eventFilter] key pressed % d event.", keyCode);
		emit win->sig_notifyKeyPressed(keyCode);

		return true;
	} else {
		return QObject::eventFilter(obj, event);
	}
	return false;
}
