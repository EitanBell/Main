/***********************************************************************
*	Developer: Eitan Bellaiche      								   *	 
*	Subject: LRU CACHE									               *
***********************************************************************/

#include <iostream>			/* cout */
#include <iterator>			/* iterator, auto */
#include <list>				/* list */
#include <unordered_map>	/* unordered_map */

template <class T> 
class LRUCache
{
public:
	explicit LRUCache(std::size_t capacity): m_capacity(capacity){}
	void Put(int key, const T& value);
	const T& Get(int key) const;
	void Print() const;

private:
	typedef typename std::list<std::pair<int, T>>::iterator iter_t;
	std::list<std::pair<int, T>> m_dlist;
	std::unordered_map<int, iter_t> m_hashMap; 
	std::size_t m_capacity;
};

template <class T>
void LRUCache<T>::Put(int key, const T& value)
{
	if (m_hashMap.find(key) == m_hashMap.end())
	{
		if (m_dlist.size() == m_capacity)
		{			
			m_hashMap.erase(m_dlist.back().first);
			m_dlist.pop_back();
		}
	}
	else
	{
		m_dlist.erase(m_hashMap[key]);
	}

	m_dlist.push_front(std::make_pair(key, value));
	m_hashMap[key] = m_dlist.begin();
}

template <class T>
const T& LRUCache<T>::Get(int key) const
{
	auto iter = m_hashMap.find(key);
	if (m_hashMap.end() == iter)
	{
		throw std::runtime_error("Key not found");
	}
	return (iter->second->second);
}

template <class T>
void LRUCache<T>::Print() const
{
	for (const auto& item : m_dlist )
	{
		std::cout << "key: " << item.first << 
		" value: " << item.second << std::endl;
	}
}


int main(void)
{
	std::size_t capacity = 7;
	std::size_t i = 0;
	
	LRUCache<int> lruCache(capacity);

    std::cout << std::endl;
	std::cout << "The LRU-Cache is empty" << std::endl;
	lruCache.Print();

	for (i = 1; i <= capacity; ++i)
	{
		lruCache.Put(i, 10 + i);
	}
	std::cout << "After Put() full capacity times: " << std::endl;
	lruCache.Print();

	lruCache.Put(2, 101);
	lruCache.Put(6, 100);

	int cache_value = lruCache.Get(2);
	if (101 == cache_value)
	{
		std::cout << std::endl << "Get() Test -  SUCCESS" << std::endl << std::endl;
	}
	
	std::cout << "After changing values for 2, 6 keys:" << std::endl;
	lruCache.Print();

	lruCache.Put(8, 8000);

	std::cout << std::endl << "After Put() new value: " << std::endl;
	lruCache.Print();

	return 0;
}
