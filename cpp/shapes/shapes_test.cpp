/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 09/04/23	                  *
*	Subject: Shapes			   		  *
*				                	  *
**************************************/	
	   			        
#include <iostream>		// cout

#include "shapes.hpp"	// shapes API

using namespace ilrd;
using std::cout;
using std::endl;

int main(void)
{
    Point p1(1, 2);
	Point p2(5, 3);
	Point p_circle(5, 1);
	Point p_square(9, 0);
	Point p_rectangle(4, 7);
	Point p_move(1, 1);
	
	Circle circle2(p_circle, 126);
    
    cout << endl;
	std::cout << "******************** TESTS BEGIN *******************"<< endl;
	
	cout << endl;
	cout << "----- Line Test: -----" << endl;
	cout << endl;
	Line line(p1, p2);
	line.Draw();
	line.Move(p_move);
	cout << "line after Move((1, 1)): (left <--> right) only" << endl;
	line.Draw();
	
	cout << endl;
	cout << "----- Rectangle Test: -----" << endl;
	cout << endl;
	Rectangle rectangle(p_rectangle, 4, 5);
	rectangle.Draw();
	rectangle.Move(p_move);
	cout << "Rectangle after Move((1, 1)): (left <--> right) only" << endl;
	rectangle.Draw();
	
	cout << endl;
	cout << "----- Square Test: -----" << endl;
	cout << endl;
	Square square(p_square, 3);
	square.Draw();
	square.Move(p_move);
	cout << "Square after Move((1, 1)): (left <--> right) only" << endl;
	square.Draw();
	
	cout << endl;
	cout << "----- Circle Test: -----" << endl;
	cout << endl;
	Circle circle(p_circle, 12);
	circle.Draw();
	circle.Move(p_move);
	cout << "Circle after Move((1, 1)): (left <--> right) only" << endl;
	circle.Draw();
	
	cout << endl;
	cout << "----- ShapeComposite Test: -----" << endl;
	cout << endl;
	ShapeComposite composite(&rectangle);
	composite.AddShape(&circle);
	composite.AddShape(&square);
	composite.Draw();
	
	cout << endl;
	cout << "----- ShapeComposite within ShapeComposite Test: -----" << endl;
	cout << endl;
	ShapeComposite comp2(&composite);
	comp2.AddShape(&circle2);
	comp2.Draw();
	comp2.Move(p_move);
	cout << endl;
	cout << "ShapeComposite after Move((1, 1)): (left <--> right) only" << endl;
	comp2.Draw();
	
	cout << endl;							     
	std::cout << "****************** TESTS FINISHED ******************"<< endl;
	cout << endl;
}


