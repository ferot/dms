/*
 * MwsrQueue.hpp
 *
 *  Created on: Sep 17, 2017
 *      Author: tf
 */

#ifndef MWSRQUEUE_HPP_
#define MWSRQUEUE_HPP_

#include <mutex>
#include <utility>
#include <condition_variable>
#include <algorithm>
#include <assert.h>

/*
 * Class implementing Multiple Writer Single Reader Queue.
 *
 * Assumed that Fixed Sized Collection (FSC) provides implementation
 * of both push, pop methods and is_full as well.
 */
template<class FSC>
class MWSRQueue {
private:
	std::mutex mx;
	std::condition_variable waitrd;
	std::condition_variable waitwr;
	FSC coll;
	bool killflag = false;
	unsigned int m_maxsize;

	//stats:
	int nfulls = 0;
	size_t hwmsize = 0; //high watermark on queue size

public:

	MWSRQueue(int maxSize = 10) :
			m_maxsize(maxSize) {
	}
	void push_back(typename FSC::value_type&& it) {
		//if the queue is full, BLOCKS until some space is freed
		{ //creating scope for lock
			std::unique_lock<std::mutex> lock(mx);
			while (isFull() && !killflag) {
				waitwr.wait(lock);
				++nfulls;
				//this will also count spurious wakeups,
				//  but they’re supposedly rare
			}

			if (killflag) {
				return;
			}
			assert(!isFull());
			coll.push_back(std::move(it));
			size_t sz = coll.size();
			hwmsize = std::max(hwmsize, sz);
		}      //unlocking mx

		waitrd.notify_one();
	}

	std::pair<bool, typename FSC::value_type> pop_front() {
		std::unique_lock<std::mutex> lock(mx);
		while (coll.size() == 0 && !killflag) {
			waitrd.wait(lock);
		}
		if (killflag)
			return std::pair<bool, typename FSC::value_type>(false,
					typename FSC::value_type());

		assert(coll.size() > 0);
		typename FSC::value_type ret = std::move(coll.front());
		coll.pop_front();
		lock.unlock();
		waitwr.notify_one();

		return std::pair<bool, typename FSC::value_type>(true, std::move(ret));
	}

	void kill() {
		{      //creating scope for lock
			std::unique_lock<std::mutex> lock(mx);
			killflag = true;
		}      //unlocking mx

		waitrd.notify_all();
		waitwr.notify_all();
	}

	bool isFull() {
		return coll.size() == m_maxsize ? true : false;
	}
};

#endif /* MWSRQUEUE_HPP_ */
