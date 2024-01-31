/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 23/04/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Simple String   		  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_SIMPLE_STRING_HPP
#define ILRD_RD136_7_SIMPLE_STRING_HPP

#include <cstddef>	/* size_t */

namespace ilrd
{
class String
{
public:
	String(const char *str_ = "");	 //non-explicit in purpose
	String(const String& src_);
	String& operator=(const String& other_);
	~String();

	size_t Length() const;
	const char *Cstr() const;

private:
    char *m_str;
};

bool operator>(const String& left, const String& right);
bool operator<(const String& left, const String& right);
bool operator==(const String& left, const String& right);
std::ostream& operator<<(std::ostream& os, const String& other_);
}

#endif	// ILRD_RD136_7_SIMPLE_STRING_HPP
