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

using std::string;
class Config {
private:
	static Config* m_instance;
	std::mutex m_mut;

	std::string m_filepath;
	std::ifstream m_read_file_handler;
	std::ofstream m_write_file_handler;
	Json::Reader m_reader;
	Json::StyledStreamWriter m_writer;

public:
	Config(Config const&) = delete;
	void operator=(Config const&) = delete;
	static Config* getInstance(std::string filename = std::string("config.ini"));


	Config(std::string);
	std::string getValue(string, string);
	bool setValue(string, string, string);
};

#endif /* CONFIG_HPP_ */
