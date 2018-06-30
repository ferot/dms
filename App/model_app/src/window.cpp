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
void Window::slot_updateModelWindow(StateObject state) {
//    t_p_coords gridCOords = convertCoordsToGridAbstract(state.getCoords());
//	//inversion of coords is essential to keep table-indexing
//    cv::Point resultPoint(gridCOords.first, gridCOords.second);

    cv::Mat& image = state.getGridHandle()->draw();
    drawAdditionalInfo(image, state);

	if (m_modelDebWinEnabled) {
		cv::imshow("MODEL_DEBUG", image);
		cv::moveWindow("MODEL_DEBUG", 10, 500);

	} else {
		cv::destroyWindow("MODEL_DEBUG");
	}
}

/**
 * @brief Window::drawAdditionalInfo paints infor about actual coords, etc
 * @param image
 */
void Window::drawAdditionalInfo(cv::Mat & image, StateObject state){
    t_p_coords coords = state.getCoords();
    int grid_w_dim = state.getGridHandle()->getGridDim().first;
    int grid_h_dim = state.getGridHandle()->getGridDim().second;


    std::string coordString = "(" + std::to_string(coords.first) + "," + std::to_string(coords.second) + ")";
    cv::putText(image, coordString , cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, 0.5,
                cv::Scalar(50, 170, 50), 2);

    std::string endCoords("(" + numToString(grid_w_dim-1) + "," + numToString(grid_h_dim-1)+ ")");
    cv::putText(image, endCoords, cv::Point(model_win_h - 50, model_win_h - 10), cv::FONT_HERSHEY_SIMPLEX,
                0.5, cv::Scalar(50, 170, 50), 2);
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
