#include "window.h"
/**
 * Constructor of app's window.
 * Sets up all widgets and connects appropriate internal signals/slots
 * @param parent
 */

std::string modelWinName = "MODEL_DEBUG";
std::string debugWinName = "DEBUG_WINDOW";


Window::Window(int camID, QWidget *parent) :
		QMainWindow(parent) {
	setFixedSize(320, 320);

	m_keyFilter = new keyReceiver();
	this->installEventFilter(m_keyFilter);

	m_debWinEnabled = false;
	m_modelDebWinEnabled = false;

	//debug window
	m_button = new QPushButton("debug_window", this);
	m_button->setGeometry(10, 10, 150, 30);
	m_button->setToolTip("Toggle on/off videostream preview");
	m_button->setCheckable(true);

	m_buttonStartTrack = new QPushButton("model_window", this);
	m_buttonStartTrack->setGeometry(10, 50, 150, 30);
	m_buttonStartTrack->setToolTip("Starts tracking");
	m_buttonStartTrack->setCheckable(true);

	createWindows(camID);

	connect(m_button, SIGNAL(clicked(bool)), this,
			SLOT(slot_debugWindowClicked(bool)));

	connect(m_buttonStartTrack, SIGNAL(clicked(bool)), this,
			SLOT(slot_modelDebugWindowClicked(bool)));


}

/**
 * Helper callback function. Manages drag/drop debug windows
 *
 * @param event
 * @param x
 * @param y
 * @param
 * @param winName
 */
static void onMouse( int event, int x, int y, int, void* winName)
{
    if( event != cv::EVENT_LBUTTONDOWN )
        return;
    cv::moveWindow((const char*)(winName), x, y);

}

/**
 * Creates named windows and sets their callbacks.
 */
void Window::createWindows(int camID){
	modelWinName+="_CAM_" + std::to_string(camID);
	debugWinName+="_CAM_" + std::to_string(camID);

	cv::setMouseCallback(debugWinName, onMouse, (void *)(debugWinName.c_str()));
	cv::setMouseCallback(modelWinName, onMouse, (void *)(modelWinName.c_str()));

}

/*** SLOTS ***/

/**
 * Slot setting debug window's enable flag and resending signal, to inform observers.
 * @param checked - param set by Qt framework on click event
 */
void Window::slot_debugWindowClicked(bool checked) {
	m_debWinEnabled = (checked == true) ? true : false;
	emit sig_notifyDebugWindow(checked);
}

/**
 * Slot emitting signal to start tracking procedure.
 * @param checked
 */
void Window::slot_modelDebugWindowClicked(bool checked) {
	m_modelDebWinEnabled = (checked == true) ? true : false;
	emit sig_notifyModelDebugWindow(checked);
}

/**
 * Slot emitting signal to switch tracking procedure.
 * @param checked
 */
void Window::slot_switchTrackerClicked(bool checked) {
	m_modelDebWinEnabled = (checked == true) ? true : false;
	emit sig_notifySwitchTrackers(checked);
}

void Window::slot_updateDebugWindow(cv::Mat frame) {
if (m_debWinEnabled) {
//	cv::imshow(debugWinName, frame);

} else {
//	cv::destroyWindow(debugWinName);
}

}

void Window::slot_updateModelDebugWindow(cv::Point2d point) {
	int w = 400, h = 400;

	cv::Mat image = cv::Mat(w, h, CV_8UC3, cv::Scalar(200, 200, 200));
	cv::circle(image, point, 5, cv::Scalar(0, 0, 255), cv::FILLED, cv::LINE_4);

	int thickness = 2;
	int lineType = cv::LINE_8;

	cv::putText(image, "Y", cv::Point(15, 20), cv::FONT_HERSHEY_SIMPLEX, 0.75,
			cv::Scalar(50, 170, 50), 2);
//	cv::putText(image, "Y", cv::Point(w - 150, 60), cv::FONT_HERSHEY_SIMPLEX,
//			0.75, cv::Scalar(50, 170, 50), 2);
	cv::putText(image, "X", cv::Point(w - 30, h - 60), cv::FONT_HERSHEY_SIMPLEX,
			0.75, cv::Scalar(50, 170, 50), 2);

	cv::line(image, cv::Point(10, 10), cv::Point(10, h - 50),
			cv::Scalar(0, 0, 0), thickness, lineType);
	cv::line(image, cv::Point(10, h - 50), cv::Point(w - 50, h - 50),
			cv::Scalar(0, 0, 0), thickness, lineType);
//	cv::line(image, cv::Point(10, h - 50), cv::Point(w - 150, 60),
//			cv::Scalar(0, 0, 0), thickness, lineType);

	if (m_modelDebWinEnabled) {
//		cv::imshow(modelWinName, image);

	} else {
//		cv::destroyWindow(modelWinName);
	}

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
