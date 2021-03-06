#include "window.h"

/**
 * Constructor of app's window.
 * Sets up all widgets and connects appropriate internal signals/slots
 * @param parent
 */

std::string debugWinName = "DEBUG_WINDOW";

Window::Window(int camID, QWidget *parent) :
		QMainWindow(parent) {
	setFixedSize(320, 320);

	m_keyFilter = new keyReceiver();
	this->installEventFilter(m_keyFilter);

    m_debWinEnabled = true;

	//debug window
	m_button = new QPushButton("debug_window", this);
	m_button->setGeometry(10, 10, 150, 30);
	m_button->setToolTip("Toggle on/off videostream preview");
	m_button->setCheckable(true);
    slot_debugWindowClicked(true);

#ifndef __arm__
	createWindows(camID);
#endif
	connect(m_button, SIGNAL(clicked(bool)), this,
			SLOT(slot_debugWindowClicked(bool)));
}

#ifndef __arm__
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
	debugWinName+="_CAM_" + std::to_string(camID);
	cv::setMouseCallback(debugWinName, onMouse, (void *)(debugWinName.c_str()));
}
#endif

/*** SLOTS ***/

/**
 * Slot setting debug window's enable flag and resending signal, to inform observers.
 * @param checked - param set by Qt framework on click event
 */
void Window::slot_debugWindowClicked(bool checked) {
	m_debWinEnabled = (checked == true) ? true : false;
	emit sig_notifyDebugWindow(checked);
}

void Window::slot_updateDebugWindow(cv::Mat frame) {
if (m_debWinEnabled) {
	cv::imshow(debugWinName, frame);

} else {
	cv::destroyWindow(debugWinName);
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
