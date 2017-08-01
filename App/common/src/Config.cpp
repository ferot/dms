/*
 * Config.cpp
 *
 *  Created on: Aug 1, 2017
 *      Author: tf
 */
#include "Config.hpp"
#include <jsoncpp/json/reader.h>

Config::Config(std::string filename = std::string("config.ini")) : m_filepath(filename){

}
bool Config::setValue(std::string val) {
	bool ret = true;
	Json::Value root;
	std::call_once(m_oflag, [&](){ m_file_handler.open(m_filepath.c_str()); }); //ensures file 'll be opend once by one thread

	std::unique_lock<std::mutex> lock(m_mut, std::defer_lock);
	//TODO: set value in appropriate section

	return ret;
}

std::string Config::getValue(std::string val) {
	std::string value;
	Json::Value root;

	std::call_once(m_oflag, [&](){ m_file_handler.open(m_filepath.c_str(), 	std::ios::in | std::ios::out); }); //ensures file 'll be opend once by one thread
//	m_file_handler >> root;

	if(!m_reader.parse(m_file_handler, root)) {
		LOGMSG_ARG(LOG_ERROR, "Couldn't parse %s config file", m_filepath.c_str());
		return value;
	}
	std::unique_lock<std::mutex> lock(m_mut);
	//TODO: get value in appropriate section
	std::string test = root["testvalue"][val].asString();
	value = test;
	return value;
}
