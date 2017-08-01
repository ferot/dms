/*
 * Config.hpp
 *
 *  Created on: Aug 1, 2017
 *      Author: tf
 */

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <jsoncpp/json/json.h>
#include <mutex>
#include <fstream>
#include "logger.h"

/*
 * Class providing interface to obtain/set values
 * from config file.
 * Settings are stored in Json format
 */
class Config {
private:
	std::mutex m_mut;
	std::once_flag m_oflag;

	std::string m_filepath;
	std::ifstream m_file_handler;
	Json::Reader m_reader;
public:
	Config(std::string);
	std::string getValue(std::string val);
	bool setValue(std::string val);
};

#endif /* CONFIG_HPP_ */
