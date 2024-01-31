/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Gal		              *
*	Date: 09/04/23	                  *
*	Subject: Shapes			   		  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_SHAPES_HPP
#define ILRD_RD136_7_SHAPES_HPP

#include <iosfwd>	// ostream
#include <list>		// <list>

namespace ilrd
{

/*-------------------------------- class Point -------------------------------*/

class Point
{
public:
	explicit Point(double x = 0, double y = 0);
	
	double GetX() const;
	double GetY() const;
	void SetX(double newPosition);
	void SetY(double newPosition);
	
private:
	double m_x;
	double m_y;
};

/*-------------------------------- class Shape -------------------------------*/

class Shape
{
public:
	//virtual bool operator==(const Shape& sh) const =0;
	
	virtual void Draw() const =0;
	virtual void Move(Point newPoint)=0;

	virtual ~Shape() {}
};

/*-------------------------------- class Line --------------------------------*/

class Line: public Shape
{
public:
	explicit Line(Point start, Point end);
	//bool operator==(const Shape& sh) const;
	
	void Draw() const;
	void Move(Point newPoint);
	
private:
	Point m_start;
	Point m_end;
};

/*------------------------------ class Rectangle -----------------------------*/

class Rectangle: public Shape
{
public:
	explicit Rectangle(Point left_up, double length, double width);
	//bool operator==(const Shape& sh) const;
	
	void Draw() const;
	void Move(Point newPoint);
	
private:
	Point m_left_up;
	double m_length;
	double m_width;
};

/*------------------------------- class Square -------------------------------*/

class Square: public Shape
{
public:
	explicit Square(Point left_up, double width);
//	bool operator==(const Shape& sh) const;

	void Draw() const;
	void Move(Point newPoint);
	
private:
	Point m_left_up;
	double m_width;
};

/*-------------------------------- class Circle ------------------------------*/

class Circle: public Shape
{
public:
	explicit Circle(Point center_point, double radius);
//	bool operator==(const Shape& sh) const;

	void Draw() const;
	void Move(Point newPoint);
	
private:
	Point m_center_point;
	double m_radius;
};

/*---------------------------- class ShapeComposite --------------------------*/

class ShapeComposite: public Shape
{
public:
	explicit ShapeComposite(Shape *sh);
//	bool operator==(const Shape& sh) const;

	void AddShape(Shape *sh);
	void RemoveShape(Shape *sh);
	
	void Draw() const;
	void Move(Point newPoint);
	
private:
    std::list<Shape*> m_comp_list;
};
} // ilrd

#endif	// ILRD_RD136_7_SHAPES_HPP

