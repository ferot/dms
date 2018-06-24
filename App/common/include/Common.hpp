/*
 * Common.hpp
 *
 *  Created on: Aug 19, 2017
 *      Author: tf
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <sys/stat.h>
#include <string.h>
#include <jsoncpp/json/json.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "logger.h"

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

template <typename T>
static inline std::string numToString(T num){
	std::stringstream ss;
	ss << num;
	return ss.str();
}

/*
 * Initializes logger instance
 */
inline int initLogger() {
	return mylog_init();
}

/**
 * @brief createDirectory if it doesn't exist.
 * @param dirname
 * @return CMN_RC_SUCCESS on success, CMN_RC_ERROR otherwise
 */
inline CommonRC createDirectory(std::string dirname) {
    CommonRC ret = CMN_RC_SUCCESS;
    const int dir_err = mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err) {
        LOGMSG_ARG(LOG_ERROR, "Error creating %s directory!", dirname.c_str());
        ret = CMN_RC_ERROR;
    }
    return ret;
}

/**
 * @brief generateTimestamp by providing format
 * @param format
 * @param date
 * @param size - size of date (must be counted manually as format changes, including '/0' char)
 */
inline void generateTimestamp(std::string format, char date[], size_t size) {
    time_t t = time(0);
    struct tm *tm;

    tm = gmtime(&t);
    strftime(date, size, format.c_str(), tm);
}

/**
 * @brief generateDateString
 * Note : if format changes remember to change size of date
 * @return string with date in format YEAR_MONTH_DAY
 */
inline std::string generateDateString() {
    const size_t size = 11;
    char date[size];

    generateTimestamp("%Y_%m_%d", date, size);
    return std::string(date);
}
#endif /* COMMON_HPP_ */
