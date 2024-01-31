/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Or	                  *
*	Date: 27/03/23	                  *
*	Valgrind & gdb tested             *
*	Subject: Complex   				  *
*				                	  *
**************************************/	
#ifndef ILRD_RD136_7_COMPLEX_HPP
#define ILRD_RD136_7_COMPLEX_HPP

#include <iosfwd> // ostream

namespace ilrd
{
class Complex;

inline const Complex operator+(const Complex& comp1_, const Complex& comp2_);
inline const Complex operator-(const Complex& comp1_, const Complex& comp2_);
inline const Complex operator*(const Complex& comp1_, const Complex& comp2_);
inline const Complex operator/(const Complex& comp1_, const Complex& comp2_);
inline bool operator==(const Complex& comp1_, const Complex& comp2_);
inline bool operator!=(const Complex& comp1_, const Complex& comp2_);
std::ostream& operator<<(std::ostream& os, const Complex& comp_);
std::istream& operator>>(std::istream& is, Complex *comp_);

class Complex
{
public:
    inline Complex(double r_ = 0, double i_ = 0);
    
    inline Complex& operator+=(const Complex& other_);
    inline Complex& operator-=(const Complex& other_);
    inline Complex& operator*=(const Complex& other_);
    inline Complex& operator/=(const Complex& other_);

    inline void SetReal(double r_);
    inline void SetImaginary(double i_);
    inline double GetReal() const;
    inline double GetImaginary() const;
   
private:
    double m_r;
    double m_i;
};

/*-------------------------------- CTORS & DTOR ------------------------------*/

inline Complex::Complex(double r_, double i_): m_r(r_), m_i(i_)
{
}

/*--------------------------------- OPERATORS --------------------------------*/

inline const Complex operator+(const Complex& comp1_, const Complex& comp2_)
{
    Complex result(comp1_);
    
    return (result += comp2_);
}

inline const Complex operator-(const Complex& comp1_, const Complex& comp2_)
{
    Complex result(comp1_);
    
    return (result -= comp2_);
}

inline const Complex operator*(const Complex& comp1_, const Complex& comp2_)
{
	Complex result(comp1_);
    
    return (result *= comp2_);
}

inline const Complex operator/(const Complex& comp1_, const Complex& comp2_)
{
	Complex result(comp1_);
    
    return (result /= comp2_);
}

inline Complex& Complex::operator+=(const Complex& other_)
{
    m_r += other_.m_r;
    m_i += other_.m_i;
    
    return *this;
}

inline Complex& Complex::operator-=(const Complex& other_)
{
    m_r -= other_.m_r;
    m_i -= other_.m_i;
    
    return *this;
}

inline Complex& Complex::operator*=(const Complex& other_)
{
    double temp_r = m_r;
    double temp_other_r = other_.m_r;
    
    m_r = temp_r * temp_other_r - m_i * other_.m_i;
    m_i = temp_r * other_.m_i + m_i * temp_other_r;
    
    return *this;
}

inline Complex& Complex::operator/=(const Complex& other_)
{
    double temp_r = m_r;
    double temp_other_r = other_.m_r;
    double denominator = other_.m_r * other_.m_r + other_.m_i * other_.m_i;
    
    m_r = (temp_r * temp_other_r + m_i * other_.m_i) / denominator;
    m_i = (m_i * temp_other_r - temp_r * other_.m_i) / denominator;
    
    return *this;
}

inline bool operator==(const Complex& comp1_, const Complex& comp2_)
{
    return ((comp1_.GetReal() == comp2_.GetReal()) && 
            (comp1_.GetImaginary() == comp2_.GetImaginary()));
}

inline bool operator!=(const Complex& comp1_, const Complex& comp2_)
{
    return !(comp1_ == comp2_);
}

std::ostream& operator<<(std::ostream& out_, const Complex& comp_)
{
    return (out_ << comp_.GetReal() << std::showpos << comp_.GetImaginary() 
																		<< "i");
}

std::istream& operator>>(std::istream& in_, Complex *comp_)
{
    double real = 0;
    double image = 0;
    
    std::cout << "Enter Real Part: ";
    in_ >> real;
    std::cout << "Enter Imaginary Part:";
    in_ >> image;
    
    comp_->SetReal(real);
    comp_->SetImaginary(image);
    
    return in_;
}

/*---------------------------------- METHODS ---------------------------------*/

inline void Complex::SetReal(double r_)
{
    m_r = r_;
}

inline void Complex::SetImaginary(double i_)
{
	m_i = i_;
}

inline double Complex::GetReal() const
{
	return m_r;
}

inline double Complex::GetImaginary() const
{
	return m_i;
}

}

#endif    // ILRD_RD136_7_COMPLEX_HPP
