/**************************************
 *	Developer: Eitan Bellaiche        *
 *	Date: 25/06/23	                  *
 *	Subject: generic hashtable	 	  *
 **************************************/

#include <iostream>     //std::cout
#include <vector>       //std::vector
#include <list>         //std::list
#include <functional>   //std::function
#include <algorithm>    //std::find_if

namespace ilrd
{

template <typename K, typename V, class HASH = std::hash<K>>
class HashTable
{
public:
    HashTable();
    ~HashTable() = default;

    void Insert(const K& key, const V& val);
    void Remove(const K& key);
    V& Find(const K& key);
    std::size_t Size() const;

private:
    std::vector<std::list<std::pair<K, V>>> m_table;
    HASH m_hashFunc;
    std::size_t m_size{0};
};

template <typename K, typename V, class HASH>
HashTable<K, V, HASH>::HashTable(): m_size(0) {}

template <typename K, typename V, class HASH>
void HashTable<K, V, HASH>::Insert(const K& key, const V& val)
{
    std::size_t index = m_hashFunc(key);

    m_table[index].push_back(std::make_pair(key, val));
    ++m_size;
}

template <typename K, typename V, class HASH>
void HashTable<K, V, HASH>::Remove(const K& key)
{
    std::size_t index = m_hashFunc(key);

	for (auto iter = m_table.begin(); iter != m_table.end(); ++iter)
	{
		if (key == iter->first)
		{
			m_table[index].erase(iter);
			--m_size;
		}
	}
}

template <typename K, typename V, class HASH>
V& HashTable<K, V, HASH>::Find(const K& key)
{
    std::size_t index = m_hashFunc(key);

	for (auto iter = m_table.begin(); iter != m_table.end(); ++iter)
	{
		if (key == iter->first)
		{
			return iter->second;
		}
	}
}

template <typename K, typename V, class HASH>
std::size_t HashTable<K, V, HASH>::Size() const
{
    return m_size;
}

} //namespace ilrd

int main(void)
{
    ilrd::HashTable<int, int, std::hash<int>> hashTable;
    
    for (int i = 0; 300 > i; ++i)
    {
        hashTable.Insert(i, (i * 100));
    }
    
    int status = 0;

    for (int i = 0; (100 > i); ++i)
    {
        status += !(hashTable.Find(i) == (i * 100));
    }

    (0 == status && 300 == hashTable.Size()) ? std::cout << "SUCCESS ✅" << std::endl: 
                                               std::cout << "FAIL ❌" << std::endl;

    for (int i = 0; 300 > i; ++i)
    {
        hashTable.Remove(i);
    }

    for (int i = 100; (0 > i); ++i)
    {
        status += (hashTable.Find(i) == (i * 100));
    }
    
    (0 == status && 0 == hashTable.Size()) ? std::cout << "SUCCESS ✅" << std::endl:
                                             std::cout << "FAIL ❌" << status <<  std::endl;

    return 0;
}
