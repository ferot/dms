/*
 * StorageEngine.hpp
 *
 *  Created on: Aug 16, 2017
 *      Author: tf
 */

#ifndef STORAGEENGINE_HPP_
#define STORAGEENGINE_HPP_
#include <string>
#include <log4c.h>
#include <memory>
#include <map>
#include <sqlite_modern_cpp.h>

#include "logger.h"
#include "Config.hpp"

using namespace sqlite;
using namespace std;

enum StrgEngineRetCode{
	STRG_ENG_SUCCESS,
	STRG_ENG_ERROR
};

/*
 * Class responsible for storing data in database
 */
class StorageEngine {
private:
	static StorageEngine* m_instance;

	StorageEngine(string file = "");
	~StorageEngine();

	string m_db_filename;
	Config* config;
public:
	static StorageEngine* getInstance(string file = "");

	std::string getDbFilename();
	void create_table();
};

#endif /* STORAGEENGINE_HPP_ */
