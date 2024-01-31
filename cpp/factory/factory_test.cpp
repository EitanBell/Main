/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Yuval		              *
*	Date: 01/06/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Factory		   		  *
*				                	  *
**************************************/		

#define TEST_RES(status) (status ? "\xe2\x9c\x97\033[0;31m  FAIL\033[0m" : "\xE2\x9C\x93 \033[0;32m SUCCESS\033[0m")
#define BOOL_TO_STATUS(bool) ((bool) ^ 1)
	   			        
#include <iostream>		// cout
#include <vector>		// vector

#include "shapes.hpp"	// shapes API
#include "factory.hpp"	// factory API

using namespace ilrd; 

class ShapesCreator
{
public:
	ShapesCreator(Point pt1, Point pt2, double len1, double len2): 
						   m_pt1(pt1), m_pt2(pt2), m_len1(len1), m_len2(len2) {}

	Line *CreateLine(){return new Line(m_pt1,m_pt2);}
	Circle *CreateCircle(){return new Circle(m_pt1, m_len1);}
	Rectangle *CreateRectangle(){return new Rectangle(m_pt1, m_len1, m_len2);}
	Square *CreateSquare(){return new Square(m_pt1, m_len1);}

private:
	Point m_pt1;
	Point m_pt2;
	double m_len1;
	double m_len2;
};

int main(void)
{
	std::cout << "\n\t******************** TESTS BEGIN ********************\n\n";
	
	Factory<Shape, std::string, ShapesCreator *> shapesFactory;
	std::vector<Shape *> shapes;

	ShapesCreator shape_creator(Point(0, 0), Point(3, 10), 5, 16);

	std::function <Line *(ShapesCreator *)> 
										 CreateLine(&ShapesCreator::CreateLine);
	std::function <Circle *(ShapesCreator *)> 
									 CreateCircle(&ShapesCreator::CreateCircle);
	std::function <Rectangle *(ShapesCreator *)> 
							   CreateRectangle(&ShapesCreator::CreateRectangle);
	std::function <Square *(ShapesCreator *)> 
									 CreateSquare(&ShapesCreator::CreateSquare);
	
	shapesFactory.Add("line", CreateLine);
	shapesFactory.Add("circle", CreateCircle);
	shapesFactory.Add("rectangle", CreateRectangle);
	shapesFactory.Add("square", CreateSquare);
	
    shapes.push_back((shapesFactory.Create("line", &shape_creator)));
	shapes.push_back((shapesFactory.Create("circle", &shape_creator)));
    shapes.push_back((shapesFactory.Create("rectangle", &shape_creator)));
    shapes.push_back((shapesFactory.Create("square", &shape_creator)));

	for (auto shape : shapes)
    {
		std::cout << "\t";
        shape->Draw();
        delete shape;
    }

	std::cout << std::endl;
	std::cout << "\t\t\tTest Result:   "<< TEST_RES(0);
								     
	std::cout << "\n\n\t******************* TESTS FINISHED *******************\n\n";
}