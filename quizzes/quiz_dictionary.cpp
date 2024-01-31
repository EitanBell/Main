/**************************************
 *	Developer: Eitan Bellaiche        *
 *	Date: 06/07/23	                  *
 *	Subject: Dictionary			 	  *
 **************************************/

#include <iostream> 		// std
#include <set>				// multiset
#include <unordered_map> 	// unordered_map
#include <string> 			// string

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) ((res) ^ (1))

namespace ilrd
{
	class Dictionary
	{
	public:
		Dictionary() = default;
  	    ~Dictionary() = default;

		void Insert(std::string key, int value); 	// O(logN) or better
		int Search(std::string key);				// O(logN) or better
		void Delete(std::string key); 				// O(logN) or better 
		int Size();									// O(1)
		int Min(); 									// O(1)
		int Max(); 									// O(1)
	private:
		std::unordered_map<std::string, std::multiset<int>::iterator> m_dict;
		std::multiset<int> m_set;		// sorted and O(logN) operations
	};

	void Dictionary::Insert(std::string key, int value)
	{
		auto newElement = m_set.insert(value);
		m_dict[key] = newElement;
	}

	int Dictionary::Search(std::string key)
	{
		return *(m_dict.at(key));
	}

	void Dictionary::Delete(std::string key)
	{
		auto elemToRemove = m_dict.find(key);
		if (elemToRemove == m_dict.end())
		{
			return;
		}

		m_set.erase(elemToRemove->second);
		m_dict.erase(key);
	}

	int Dictionary::Size()
	{
		return m_set.size();
	}

	int Dictionary::Min()
	{
		return (*m_set.begin());
	}

	int Dictionary::Max()
	{
		return (*m_set.rbegin());
	}
} // namespace ilrd;

using namespace ilrd;


int main(void)
{
    Dictionary dict;
	int status = 0;

    dict.Insert("Eitan", 5);
    dict.Insert("Bell", 10);
    dict.Insert("RD136/7", 3);

    status += (dict.Size() == 3);

    status += (dict.Search("Eitan") == 5);
    status += (dict.Search("Bell") == 10);
    status += (dict.Search("RD136/7") == 3);

    status += (dict.Min() == 3);
    status += (dict.Max() == 10);

    dict.Delete("RD136/7");
    status += (dict.Size() == 2);

    (status == 7) ? std::cout << "Test result: " << TEST_RES(0) << std::endl :
					std::cout << "Test result: " << TEST_RES(1) << std::endl;

    return 0;
}
