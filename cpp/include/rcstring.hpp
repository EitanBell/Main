/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Moriah	              *
*	Date: 01/04/23	                  *
*	Valgrind & gdb tested             *
*	Subject: RCString		   		  *
*				                	  *
**************************************/
#ifndef ILRD_RD136_7_RCSTRING_HPP
#define ILRD_RD136_7_RCSTRING_HPP

#include <iostream> // ostream

namespace ilrd
{

class RCString
{
public:
    RCString(const char *str_ = "");
    RCString(const RCString& src);
    RCString& operator=(const RCString& other);
    ~RCString();
    
    const char& operator[](std::size_t index) const;
    char &operator[](std::size_t index);
    std::size_t Length() const;
    const char *Cstr() const;
    
private:
	struct SharedString
	{
		SharedString(const char *str_ = "");	//non-explicit in purpose
		
		std::size_t m_counter;
		char m_str[1];
	};
	
	struct SharedString *m_sharedStr;
	
	SharedString *AllocShrStr(const char *str_);
	void DeleteAlloc(void);
	void ProtectSelfAssignment(const RCString& other);
};

bool operator==(const RCString& one, const RCString& other);
bool operator>(const RCString& one, const RCString& other);
bool operator<(const RCString& one, const RCString& other);
std::ostream& operator<<(std::ostream& os, const RCString& other);

} //namespace ilrd

#endif    // ILRD_RD136_7_RCSTRING_HPP


