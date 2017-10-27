/*
 * Common.hpp
 *
 *  Created on: Aug 19, 2017
 *      Author: tf
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_
#include <string.h>
#include <jsoncpp/json/json.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "logger.h"


template <typename T>
static inline std::string numToString(T num){
	std::stringstream ss;
	ss << num;
	return ss.str();
}

/*
 * Initializes logger instance
 */
static int initLogger() {
	return mylog_init();
}

/*
 * Common Return Code.
 * Represents status of operation
 */
enum CommonRC{
	CMN_RC_SUCCESS,
	CMN_RC_ERROR,
};

/*
 * Vision Engine Return Code.
 * Represents status of operation
 */
enum TrcEnRc{
	TRCK_ENG_SUCCESS,
	TRCK_ENG_ERROR,
	TRCK_ENG_VIDDEV_ERROR
};
#endif /* COMMON_HPP_ */
