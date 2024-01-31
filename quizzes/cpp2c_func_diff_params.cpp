/***********************************************************************
*	Developer: Eitan Bellaiche      								   *	 
*	Subject: Cpp2c - Func That Receives Different Params               *
***********************************************************************/

#include <iostream>		/* cout */

using namespace std;

namespace ilrd
{ 
class Person
{
public:
	explicit Person();
	~Person();
	
	Person(const Person& other);
	int GetId() const;
	virtual void Draw();

private:
	int m_id;
	static int s_count;
	
	Person& operator=(const Person& other); 
};

int Person::s_count = 0;

Person::Person(): m_id(++s_count)
{
	cout << "Person Ctor: " << m_id << endl;
}

Person::~Person()
{
	cout << "Person Dtor: " << m_id << endl;
}

int Person::GetId() const
{
	return this->m_id;
}

Person::Person(const Person& other)
{
	m_id = other.GetId();
	(void)other;
	cout << "Person CCtor: " << m_id << endl;
}

void Person::Draw()
{
	cout << "Person Draw: " << m_id << endl;
}

} //namespace ilrd

void Foo(ilrd::Person p1, ilrd::Person& p2)
{
	p1.Draw();
	p2.Draw();
}

using namespace ilrd;

int main(void)
{
	Person p1;
	Person p2;
	Person &p_ref = p2;

	Foo(p1, p_ref);

	return 0;
}

