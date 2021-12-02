//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : FIFO队列
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef FRAMEWORK_UTILS_FIFO_FIFO_H
#define FRAMEWORK_UTILS_FIFO_FIFO_H

#include <deque>
#include "utils/lock/rw_lock.h"
using namespace framework::utils::lock;

template <class T>
class Fifo
{
public:
	Fifo(const unsigned long long cap = 0xFFFF) 
		: capacity{ cap }
	{}

	~Fifo(void)
	{}

public:
	void add(const T e)
	{
		const unsigned long long itemNumber{ queue.size() };
		if (capacity > itemNumber)
		{
			//WriteLock wl{ mtx };
			mtx.lock();
			queue.push_back(e);
			mtx.unlock();
		}
	}

	void remove(void)
	{
		//WriteLock wl{ mtx };
		mtx.lock();
		typename std::deque<T>::iterator it{ queue.begin() };
		if (queue.end() != it)
		{
			queue.erase(it);
		}
		mtx.unlock();
	}
	
	T front(void)
	{
		T e{};

		mtx.lock();
		const unsigned long long itemNumber{ queue.size() };
		if (0 < queue.size())
		{
			//ReadLock rl{ mtx };
			e = queue[0];
		}
		mtx.unlock();

		return e;
	}

	const unsigned long long size(void)
	{
		//ReadLock rl{ mtx };
		return queue.size();
	}

	void clear()
	{
		//WriteLock wl{ mtx };
		mtx.lock();
		queue.clear();
		mtx.unlock();
	}

private:
	unsigned long long capacity;
	std::deque<T> queue;
	SharedMutex mtx;
};//class Fifo

#endif//FRAMEWORK_UTILS_FIFO_FIFO_H
