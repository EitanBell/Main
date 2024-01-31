/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 09/04/23	                  *
*	Subject: Shapes			   		  *
*				                	  *
**************************************/

#include <iostream>		// cout
#include <algorithm>	// for_each

#include "shapes.hpp"	// shapes API 

namespace ilrd
{

/*------------------------------------ Point ---------------------------------*/

Point::Point(double x, double y): m_x(x), m_y(y) {}

double Point::GetX() const
{
	return m_x;
}

double Point::GetY() const
{
	return m_y;
}

void Point::SetX(double newPosition)
{
	m_x = newPosition;
}

void Point::SetY(double newPosition)
{
	m_y = newPosition;
}

std::ostream& operator<<(std::ostream& os, const Point& point)
{
    os << "(" << point.GetX() << "," << point.GetY() << ")";
	
	return os;
}

/*------------------------------------ Line ----------------------------------*/

Line::Line(Point start, Point end): m_start(start), m_end(end) {}

void Line::Draw() const
{
	std::cout << "line: start: " << m_start << ", end: " << m_end << std::endl;
}

void Line::Move(Point newPoint)
{
	double new_x_start = newPoint.GetX() + m_start.GetX();
	double new_x_end = newPoint.GetX() + m_end.GetX();

	m_start.SetX(new_x_start);
	
	m_end.SetX(new_x_end);
}

/*--------------------------------- Rectangle --------------------------------*/

Rectangle::Rectangle(Point left_up, double length, double width):
						 m_left_up(left_up), m_length(length), m_width(width) {}

void Rectangle::Draw() const
{
	std::cout << "Rectangle: left-up point: " << m_left_up;
	std::cout << ", length: " << m_length << ", width: " << m_width
																   << std::endl;
}

void Rectangle::Move(Point newPoint)
{
	m_left_up.SetX(m_left_up.GetX() + newPoint.GetX());
}

/*----------------------------------- Square ---------------------------------*/

Square::Square(Point left_up, double width): 
										   m_left_up(left_up), m_width(width) {}

void Square::Draw() const
{
	std::cout << "Square: left-up point: " << m_left_up;
	std::cout << ", width: " << m_width << std::endl;
}

void Square::Move(Point newPoint)
{
	m_left_up.SetX(m_left_up.GetX() + newPoint.GetX());	
}

/*----------------------------------- Circle ---------------------------------*/

Circle::Circle(Point center_point, double radius): 
							   m_center_point(center_point), m_radius(radius) {}

void Circle::Draw() const
{
	std::cout << "Circle: center point: " << m_center_point;
	std::cout << ", radius: " << m_radius << std::endl;
}

void Circle::Move(Point newPoint)
{
	m_center_point.SetX(m_center_point.GetX() + newPoint.GetX());
}

/*------------------------------ ShapeComposite ------------------------------*/

ShapeComposite::ShapeComposite(Shape *sh)
{
	AddShape(sh);
}

void ShapeComposite::AddShape(Shape *sh) 
{
	m_comp_list.push_back(sh);
}

void ShapeComposite::RemoveShape(Shape *sh) 
{
	(void)sh; 	//Advanced
}

void ShapeComposite::Draw() const
{
	std::for_each(m_comp_list.begin(), m_comp_list.end(), 
													std::mem_fun(&Shape::Draw));
} 

void ShapeComposite::Move(Point newPoint) 
{
	std::for_each(m_comp_list.begin(), m_comp_list.end(), 
							std::bind2nd(std::mem_fun(&Shape::Move), newPoint));
}

} // ilrd namespace


