//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-28
//		Description : 单链表
//
//		History:
//					1. 2021-12-28 由王科威创建
//

#ifndef FRAMEWORK_UTILS_SINGLY_LINKED_LIST_H
#define FRAMEWORK_UTILS_SINGLY_LINKED_LIST_H

template <class T>
class SinglylinkedList
{
public:
	SinglylinkedList(void) 
	{}

	~SinglylinkedList(void)
	{}

public:
	void append(T t, Value v)
	{
		WriteLock wl{mtx};
		items.insert(std::make_pair(k, v));
	}

	void replace(Key k, Value v)
	{
		WriteLock wl{mtx};
		typename boost::unordered_map<Key, Value>::iterator it{ items.find(k) };

		if (items.end() != it)
		{
			it->second = v;
		}
		else
		{
			items.insert(std::make_pair(k, v));
		}
	}

	void remove(Key k)
	{
		WriteLock wl{mtx};
		typename boost::unordered_map<Key, Value>::iterator it{ items.find(k) };

		if (items.end() != it)
		{
			items.erase(it);
		}
	}

	std::vector<Key> keies(void)
	{
		std::vector<Key> ret;
		ReadLock rl{mtx};
		typename boost::unordered_map<Key, Value>::iterator it{items.begin()};

		for (; items.end() != it; ++it)
		{
			ret.push_back(it->first);
		}
		
		return std::move(ret);
	}

	std::vector<Value> values(void)
	{
		std::vector<Value> ret;
		ReadLock rl{mtx};
		typename boost::unordered_map<Key, Value>::iterator it{items.begin()};

		for (; items.end() != it; ++it)
		{
			ret.push_back(it->second);
		}
		
		return std::move(ret);
	}

	void clear(void)
	{
		items.clear();
	}
	
	Value at(Key k)
	{
		Value ret{};
		ReadLock rl{mtx};
		typename boost::unordered_map<Key, Value>::const_iterator it{ items.find(k) };

		if (items.end() != it)
		{
			ret = it->second;
		}

		return ret;
	}

private:
	SinglylinkedList* node;
};//class UnorderedMap

#endif//FRAMEWORK_UTILS_MAP_UNORDERED_MAP_H

