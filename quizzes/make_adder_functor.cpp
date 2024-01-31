/**************************************
*	Developer: Eitan Bellaiche        *	 
*	Subject: Make adder               *
**************************************/

#include <iostream>

using namespace std;

template <class T> 
class Adder
{
public:
	Adder(T add): m_num(add){}
	T operator ()(T number) const
	{
		return (m_num + number);
	}
private:
	T m_num;
};
 
 
int main(void)
{
	int status = 0;
	Adder<int> add_4 = Adder<int>(4);
	Adder<int> add_7 = Adder<int>(7);
	status += ((9 == add_4(5)) && (12 == add_4(8) && (10 == add_7(3))));
	
	if (1 == status)
	{
		cout << endl << "Make adder Test - SUCCESS" << endl << endl;
	}
	else
	{
		cout << endl << "Make adder Test - FAILED" << endl << endl;
	}
}

