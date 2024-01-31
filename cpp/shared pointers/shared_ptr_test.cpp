/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Ariel         	      *
*	Date: 14/05/23	                  *
*	Subject: Shared Ptr			      *
*				                	  *
**************************************/

#include <iostream> // ostream
#include <string>   // string

#include "shared_ptr.hpp" // Scope Lock API 

using namespace ilrd;
using namespace std;

class Base
{
public:
	Base(int x = 0): m_x(x) {}

	int GetX() {return m_x;}

private:
	int m_x;
};

class Derived: public Base
{
public:
	Derived(int x = 0, int y = 0): Base(x), m_y(y) {}

	int GetY() {return m_y;}

private:
	int m_y;
};

void SharedPtrTest();
/*------------------------------ Shared Ptr TEST -----------------------------*/

int main(void)
{
    SharedPtrTest();

	return 0;
}

void SharedPtrTest()
{
    SharedPtr<int> s_ptr(new int(10));
    SharedPtr<int> s_ptr2(s_ptr);

    (*s_ptr == *s_ptr2) ? (cout << "CCtor and operator* TEST - SUCCESS") : 
                                    (cout << "CCtor and operator* TEST - FAIL");

    cout << endl;
    *s_ptr2 = 20;

    ilrd::SharedPtr<int> s_ptr3 = s_ptr2;
    (*s_ptr3 == 20) ? (cout << "copy assignment TEST - SUCCESS") : 
                                        (cout << "copy assignment TEST - FAIL");
    cout << endl;

    SharedPtr<Derived> derivedSP(new Derived(10,20));
    for (size_t i = 0; 5 > i; ++i)
	{
		SharedPtr<Base> baseSP(derivedSP);

		cout << ((baseSP->GetX() == 10) ? 
                              " new Derived - SUCCESS" : " new Derived - FAIL");
        cout << endl;
    }

    SharedPtr<Base> baseSPtrs[5];
    for (size_t i = 0; 5 > i; ++i)
	{
		baseSPtrs[i] = derivedSP;

        cout << ((baseSPtrs[i]->GetX() == 10) ? 
                                            " Base - SUCCESS" : " Base - FAIL");
        cout << endl;
	}

    SharedPtr<Derived> derivedSPtrs[5];
	for (size_t i = 0; 5 > i; ++i)
	{
		derivedSPtrs[i] = derivedSP;

        cout << ((derivedSPtrs[i]->GetX() == 10) ? 
                                      " Derived - SUCCESS" : " Derived - FAIL");
        cout << endl;
        cout << ((derivedSPtrs[i]->GetY() == 20) ? 
                                      " Derived - SUCCESS" : " Derived - FAIL");
        cout << endl;
	}

    cout << endl;
}