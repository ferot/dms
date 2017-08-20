/*
 * Common.hpp
 *
 *  Created on: Aug 19, 2017
 *      Author: tf
 */

#ifndef COMMON_HPP_
#define COMMON_HPP_
#include <string.h>

template <typename T>
static std::string numToString(T num){
	std::stringstream ss;
	ss << num;
	return ss.str();
}


#endif /* COMMON_HPP_ */
