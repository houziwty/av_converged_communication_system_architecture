//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-24
//		Description : 映射表
//
//		History:
//					1. 2021-11-24 由王科威创建
//

#ifndef FRAMEWORK_UTILS_MAP_UNORDERED_MAP_H
#define FRAMEWORK_UTILS_MAP_UNORDERED_MAP_H

#include <vector>
#include "boost/unordered_map.hpp"
#include "utils/lock/rw_lock.h"
using namespace framework::utils::lock;

template <class Key, class Value>
class UnorderedMap
{
public:
	UnorderedMap(void) 
	{}

	~UnorderedMap(void)
	{
		items.clear();
	}

public:
	void add(Key k, Value v)
	{
		//WriteLock wl{ mtx };
		mtx.lock();
		items.insert(std::make_pair(k, v));
		mtx.unlock();
	}

	void replace(Key k, Value v)
	{
		typename boost::unordered_map<Key, Value>::iterator it{ items.find(k) };

		//WriteLock wl{ mtx };
		mtx.lock();
		if (items.end() != it)
		{
			it->second = v;
		}
		else
		{
			items.insert(std::make_pair(k, v));
		}
		mtx.unlock();
	}

	void remove(Key k)
	{
		typename boost::unordered_map<Key, Value>::iterator it{ items.find(k) };

		//WriteLock wl{ mtx };
		mtx.lock();
		if (items.end() != it)
		{
			items.erase(it);
		}
		mtx.unlock();
	}

	std::vector<Key> keies(void)
	{
		mtx.lock();
		std::vector<Key> ret;
		typename boost::unordered_map<Key, Value>::iterator it{items.begin()};

		//ReadLock rl{ mtx };
		for (; items.end() != it; ++it)
		{
			ret.push_back(it->first);
		}
		mtx.unlock();
		
		return std::move(ret);
	}

	void clear(void)
	{
		//WriteLock wl{ mtx };
		items.clear();
	}
	
	Value at(Key k)
	{
		mtx.lock();
		Value ret{};
		typename boost::unordered_map<Key, Value>::const_iterator it{ items.find(k) };

		//ReadLock rl{ mtx };
		if (items.end() != it)
		{
			ret = it->second;
		}
		mtx.unlock();

		return ret;
	}

private:
	boost::unordered_map<Key, Value> items;
	SharedMutex mtx;
};//class UnorderedMap

#endif//FRAMEWORK_UTILS_MAP_UNORDERED_MAP_H

