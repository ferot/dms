/*
 * Config.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: tf
 */
#include "Config.hpp"
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>

Config* Config::m_instance = nullptr;

/*
 * Returns instance of engine if it already exists.
 * In other case creates new one, with provided (or default) params.
 */
Config* Config::getInstance(string filename) {
	if (!m_instance) {
		m_instance = new Config(filename);
	}
	return m_instance;
}

/*
 * Default Constructor. Initializes object with proper filename string.
 */
Config::Config(string filename) :
		m_filepath(filename) {
}


/*
 * Method allowing to set value for specific section, and key.
 * returns true on success, otherwise false.
 */
bool Config::setValue(string section, string key, string val) {
	bool ret = false;
	Json::Value root;
	std::unique_lock<std::mutex> lock(m_mut);

	//read conf file to root Json object
	m_read_file_handler.open(m_filepath.c_str());
	if (m_read_file_handler.fail()) {
		LOGMSG_ARG(LOG_ERROR, "Error on attempt to open %s conf file!",
				m_filepath.c_str());
	} else {
		if (!m_reader.parse(m_read_file_handler, root)) {
			LOGMSG_ARG(LOG_ERROR, "Couldn't parse %s config file",
					m_filepath.c_str());
			m_read_file_handler.close();

		} else {
			m_read_file_handler.close();
			m_write_file_handler.open(m_filepath.c_str());

			if (m_write_file_handler.fail()) {
				LOGMSG_ARG(LOG_ERROR, "Error on attempt to open %s conf file!",
						m_filepath.c_str());
			} else {
				root[section][key] = val;
				m_writer.write(m_write_file_handler, root);
				ret = true;
				m_write_file_handler.close();
			}
		}
	}

	return ret;
}

/*
 * Method allowing to get value for specific section, and key.
 * returns value on success, otherwise empty string.
 */
string Config::getValue(string section, string key) {
	string value;
	Json::Value root;

	std::unique_lock<std::mutex> lock(m_mut);

	m_read_file_handler.open(m_filepath.c_str());

	if (m_read_file_handler.fail()) {
		LOGMSG_ARG(LOG_ERROR, "Error on attempt to open %s conf file!",
				m_filepath.c_str());
	} else {
		if (!m_reader.parse(m_read_file_handler, root)) {
			LOGMSG_ARG(LOG_ERROR, "Couldn't parse %s config file",
					m_filepath.c_str());
			m_read_file_handler.close();
			return value;
		}
		value = root[section][key].asString();
		m_read_file_handler.close();
	}

	return value;
}
