/*
 * StorageEngine.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: tf
 */
#include <iostream>
#include <utility>
#include <map>
#include <time.h>

#include "StorageEngine.hpp"
#include "Common.hpp"

const int TIME_STRING_LENGTH = 20;
//Used only in generateDateTime
char timestampBuffer[TIME_STRING_LENGTH] = { 0 };

/*
 * Constructor of Storage engine.
 * Creates fundamental objects essential for database Storage
 */
StorageEngine::StorageEngine(string db_file) {
	if (db_file.empty()) {
		config->getInstance();
		m_db_filename = config->getValue("Storage", "db_path");
	} else {
		m_db_filename = db_file;
	}
}

/*
 * Cleans up connections handlers as they're allocated manually
 */
StorageEngine::~StorageEngine() {
}

/*
 * Returns SQLite expected date string format "YYYY-MM-DD HH:MM:SS"
 * Date is based on local time.
 */
const char * generateDateTime() {
	time_t rawtime;
	struct tm *currentTime;
	time(&rawtime);

	currentTime = localtime(&rawtime);
	strftime(timestampBuffer, TIME_STRING_LENGTH, "%Y-%m-%d %H:%M:%S",
			currentTime);

	return timestampBuffer;
}

/*
 * Creates sqlite3 tables if not existing.
 */
void StorageEngine::create_table() {
	database db(m_db_filename);
	try {
		db << "create table if not exists events ("
				"   _id integer primary key autoincrement not null,"
				"   timestamp date,"
				"   nodename text,"
				"   objectid int"
				");";
	} catch (exception& e) {
		LOGMSG_ARG(LOG_ERROR,
				"Exception %s when trying to create database tables !",
				e.what());
	}
}

/*
 * Returns database's full path with filename
 */
std::string StorageEngine::getDbFilename() {

	return m_db_filename;
}
