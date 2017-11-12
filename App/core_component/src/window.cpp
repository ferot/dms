#include "window.h"
/**
 * Constructor of app's window.
 * Sets up all widgets and connects appropriate internal signals/slots
 * @param parent
 */
Window::Window(QWidget *parent) :
		QMainWindow(parent) {
	setFixedSize(640, 480);

	m_keyFilter = new keyReceiver();
	this->installEventFilter(m_keyFilter);

	m_debWinEnabled = false;
	//debug window
	m_button = new QPushButton("Debug Window", this);
	m_button->setGeometry(10, 10, 150, 30);
	m_button->setToolTip("Toggle on/off videostream preview");
	m_button->setCheckable(true);

	m_buttonStartTrack = new QPushButton("StartTracker", this);
	m_buttonStartTrack->setGeometry(10, 50, 150, 30);
	m_buttonStartTrack->setToolTip("Starts tracking");
	m_buttonStartTrack->setCheckable(true);

	connect(m_button, SIGNAL(clicked(bool)), this,
			SLOT(slot_debugWindowClicked(bool)));

	connect(m_buttonStartTrack, SIGNAL(clicked(bool)), this,
			SLOT(slot_startTracking(bool)));

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
void Window::slot_startTracking(bool checked) {
	emit sig_notifyStartTracking(checked);
}

void Window::slot_updateDebugWindow(cv::Mat frame) {
	if (m_debWinEnabled) {
		cv::imshow("DEBUG", frame);
		cv::moveWindow("DEBUG", 1300, 20);

	} else {
		cv::destroyWindow("DEBUG");
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
