/***********************************************************************
*	Developer: Eitan Bellaiche      								   *	 
*	Subject: List with given API						               *
***********************************************************************/
#include <iostream>		// cout

#define TEST_RES(status) (status ? "\xe2\x9c\x97 \033[0;31mFAIL\033[0m":\
					   			        "\xE2\x9C\x93 \033[0;32mSUCCESS\033[0m")
#define BOOL_TO_STATUS(res) ((res) ^ (1))

int Test(void);

namespace ilrd
{

/*--------------------------------- Node -------------------------------------*/

class Node
{
public:
    Node();
    void Tie(Node *prev);
    void Untie();
    Node& Next() const;

private:
    Node *m_next;
};

/*----------------------------------- List -----------------------------------*/

class List
{
public:
    List();
    ~List();
    
    void InsertFirst(Node *node);
    void InsertLast(Node *node);
    const Node *GetFirst() const;
    const Node *GetLast() const;
    std::size_t GetNumOfElements() const;
    void Remove(Node *node);

private:
    Node m_head;
    Node m_last;
    std::size_t m_size;
};

/*--------------------------- Node Implementation ----------------------------*/

Node::Node(): m_next(0) {}

void Node::Tie(Node *prev)
{
    prev->m_next = this;
}

void Node::Untie()
{
    m_next = 0;
}

Node& Node::Next() const
{
    return *m_next;
}

/*--------------------------- List Implementation ----------------------------*/

List::List(): m_size(0)
{
    m_last.Tie(&m_head);
}

List::~List()
{
    Node *curr = &m_head;
    Node *next = &m_head.Next();

    while (0 != next)
    {
        curr->Untie();
        --m_size;

        curr = next;
        next = &next->Next();
    }
}

void List::InsertFirst(Node *node)
{   
    Node *first_node = &m_head.Next();
    
    node->Tie(&m_head);
    first_node->Tie(node);

    ++m_size;
}

void List::InsertLast(Node *node)
{
    node->Tie(&m_last);
    m_last = *node;

    ++m_size;
}

const Node *List::GetFirst() const
{
    return &m_head.Next();
}

const Node *List::GetLast() const
{
    return &m_last;
}

std::size_t List::GetNumOfElements() const
{
    return m_size;
}

void List::Remove(Node *node)
{
    Node *next_of_next = 0;
    Node *next = 0;

    next = &node->Next();
    
    if (0 != next)
    {
        next_of_next = &next->Next();
        *node = *next;

        next_of_next->Tie(const_cast<Node *>(node));
        next->Untie();
    }
    else
    {
        node = next;
    }

    --m_size;
}

} // ilrd

/*--------------------------------- main -------------------------------------*/

using namespace ilrd;

int main(void)
{
	std::cout << "\n\t******************** TESTS BEGIN *******************\n\n";
	
	std::cout << "\t\t      List API Test:   " << TEST_RES(Test()) << std::endl;
							
	std::cout << "\n\t****************** TESTS FINISHED ******************\n\n";
	
	return 0;
}

int Test(void)
{
	int status = 0;

	List list;
    Node nodes[10];

	for (int i = 0; i < 5; ++i)
    {
        list.InsertLast(&nodes[i]);
    } 

	status += (5 == list.GetNumOfElements());

	for (int i = 5; i < 10; ++i)
    {
        list.InsertFirst(&nodes[i]);
    } 

	status += (10 == list.GetNumOfElements());
	status += (list.GetFirst() == &nodes[9]);

	for (int i = 0; i < 10; ++i)
    {
        list.Remove(&nodes[i]);
    }

	status += (0 == list.GetNumOfElements());
	
	return BOOL_TO_STATUS(status == 4);
}