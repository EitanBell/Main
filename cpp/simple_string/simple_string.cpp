/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: 	                      *
*	Date: 23/03/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Simple String   		  *
*				                	  *
**************************************/
#include <cstring>		// strcpy, strlen, memcpy
#include <iostream>		// std
#include <cassert> 		// assert

#include "simple_string.hpp"	// simple_string API 

namespace ilrd
{

/*------------------------------- HELP FUNCS ---------------------------------*/

char *StrDup(const char *str)
{
	assert(str);
	
	size_t str_len = strlen(str) + 1;
	char *dest = new char[str_len];
	
	return (char *)(memcpy(dest, str, str_len));
}

/*-------------------------------- CTORS & DTOR ------------------------------*/

String::String(const String& src_): m_str(StrDup(src_.m_str)){}

String::String(const char *str_): m_str(StrDup(str_)){}

String::~String()
{
    delete[] m_str;
    m_str = 0;
}

/*--------------------------------- OPERATORS --------------------------------*/

String& String::operator=(const String& other_)
{
    assert(other_.Cstr());
    
    char *tmp = other_.m_str;
    
    delete[] m_str;
    m_str = StrDup(tmp);
    
    return (*this);
}

/*---------------------------------- METHODS ---------------------------------*/

size_t String::Length() const
{
    assert(m_str);
    
    return ::strlen(m_str);
}

const char *String::Cstr() const
{
    return m_str;
}

/*------------------------------- GLOBAL FUNCS -------------------------------*/

std::ostream& operator<<(std::ostream& os, const String& other_)
{
    os << other_.Cstr();
    
    return os;
}

bool operator==(const String& left, const String& right)
{
	return (0 == ::strcmp(left.Cstr(), right.Cstr()));
}

bool operator<(const String& left, const String& right)
{
    return (0 > ::strcmp(left.Cstr(), right.Cstr()));
}

bool operator>(const String& left, const String& right)
{
	return (!((left < right) || (left == right)));
}

}
