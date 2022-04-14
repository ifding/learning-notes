#ifndef __COMPLEX__
#define __COMPLEX__


template<typename T>
class Complex
{
private:
    /* data */
    T re, im;
    
public:
    Complex<T>(T r = 0.0, T i = 0.0): re(r), im(i) { }
    Complex<T>(const Complex<T>& rhs): re(rhs.re), im(rhs.im) { }
    Complex<T> operator - () const;
    Complex<T>& operator += (const Complex<T>&);
    T real() const { return re; }
    T imag() const { return im; }
};


template<typename T>
inline Complex<T>&
Complex<T>::operator += (const Complex& rhs)
{
    re += rhs.re;
    im += rhs.im;
    return *this; 
}

template<typename T>
inline Complex<T>
Complex<T>::operator - () const
{
    return Complex(-re, -im);
}

template<typename T>
inline Complex<T>
operator + (const Complex<T>& x, const Complex<T>& y)
{
    return Complex(x.real() + y.real(), x.imag() + y.imag());
}


#include <iostream>

using std::ostream;

template<typename T>
ostream&
operator << (ostream& os, const Complex<T>& x)
{
    return os << '(' << x.real() << ',' << x.imag() << ')';
}

#endif