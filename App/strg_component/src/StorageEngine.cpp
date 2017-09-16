/*
 * StorageEngine.cpp
 *
 *  Created on: Aug 16, 2017
 *      Author: tf
 */
#include <iostream>
#include <utility>
#include <map>

#include "StorageEngine.hpp"
#include "Common.hpp"

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

void StorageEngine::create_table() {
	database db(m_db_filename);
	// executes the query and creates a 'user' table
	db << "create table if not exists user ("
			"   _id integer primary key autoincrement not null,"
			"   age int,"
			"   name text,"
			"   weight real"
			");";

	db << "insert into user (age,name,weight) values (?,?,?);" << 20 << u"bob"
	<< 83.25;
	int age = 21;
	float weight = 68.5;
	string name = "jack";
	// you can also extract multiple column rows
	db << "select age, name from user where _id=1;" >> tie(age, name);
	cout << "Age = " << age << ", name = " << name << endl;
}
