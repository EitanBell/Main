/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriah	              *
*	Date: 01/04/23	                  *
*	Valgrind & gdb tested             *
*	Subject: RCString		   		  *
*				                	  *
**************************************/
#include <cstring>		// strlen, memcpy
#include <cstddef>		// offsetof
#include <cassert>  	// assert

#include "rcstring.hpp"	// rcstring API 

namespace ilrd
{

/*-------------------------------- CTORS & DTOR ------------------------------*/

RCString::RCString(const char *str_): m_sharedStr(AllocShrStr(str_)){}

RCString::RCString(const RCString& src): m_sharedStr(src.m_sharedStr)
{
    ++(m_sharedStr->m_counter);
}

RCString::~RCString()
{
	DeleteAlloc();
}

/*--------------------------------- OPERATORS --------------------------------*/

RCString& RCString::operator=(const RCString& other)
{
    if (m_sharedStr != other.m_sharedStr)  
    {
        DeleteAlloc();
        m_sharedStr = other.m_sharedStr;
        ++(m_sharedStr->m_counter);
    }
    
    return *this;
}

bool operator==(const RCString& one, const RCString& other)
{
	return (0 == strcmp(one.Cstr(), other.Cstr()));
}

bool operator>(const RCString& one, const RCString& other) 
{
	return (0 < strcmp(one.Cstr(), other.Cstr())); 
}

bool operator<(const RCString& one, const RCString& other) 
{
	return (0 > strcmp(one.Cstr(), other.Cstr()));
}

std::ostream& operator<<(std::ostream& os, const RCString& other)
{
    return os << other.Cstr();
}

const char& RCString::operator[](std::size_t index) const
{
    return m_sharedStr->m_str[index];
}

char& RCString::operator[](std::size_t index)
{	
	if (1 < m_sharedStr->m_counter)
	{
		--(m_sharedStr->m_counter);

		SharedString *temp = AllocShrStr(this->Cstr());
		m_sharedStr = temp;
	}
	
	return (m_sharedStr->m_str[index]);
}

/*---------------------------------- METHODS ---------------------------------*/

size_t RCString::Length() const
{
	return strlen(m_sharedStr->m_str);
}

const char *RCString::Cstr() const
{
	return m_sharedStr->m_str;
}

/*-------------------------------- HELPER FUNCS ------------------------------*/

RCString::SharedString *RCString::AllocShrStr(const char *str_)
{
    assert(NULL != str_);
	
	size_t str_len = strlen(str_) + 1;
	size_t offset = offsetof(SharedString, m_str);
	void *sharedStr = operator new(offset + str_len);

	return (new(sharedStr) SharedString(str_));
}

void RCString::DeleteAlloc(void)
{
	if (1  < m_sharedStr->m_counter)
    {
    	--(m_sharedStr->m_counter);
    } 
   	else
   	{
		delete m_sharedStr;
    	m_sharedStr = 0;
   	}
}

void RCString::ProtectSelfAssignment(const RCString& other)
{
	++(other.m_sharedStr->m_counter);
}

/*------------------------------ SharedString --------------------------------*/

/*-------------------------------- CTORS & DTOR ------------------------------*/

RCString::SharedString::SharedString(const char *str_) : m_counter(1)
{
    size_t str_len = std::strlen(str_) + sizeof(char);
    memcpy(m_str, str_, (str_len * sizeof(char)));
}

}

