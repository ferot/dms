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
	void create_table();

	string m_db_filename;
	Config* config;

	std::shared_ptr<database> m_db;
public:
	static StorageEngine* getInstance(string file = "");
    const char * generateDateTime();
    void performStatement(std::string statement);

	std::string getDbFilename();
	std::shared_ptr<database> getDBHandle() { return m_db; };
};

#endif /* STORAGEENGINE_HPP_ */
