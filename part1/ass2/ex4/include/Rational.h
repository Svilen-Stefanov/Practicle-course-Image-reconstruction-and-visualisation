#ifndef RATIONAL_H
#define RATIONAL_H
#include<iostream>

class Rational
{
    public:
      Rational();
      Rational(int, int);
      void setNum(int);
      void setDenom(int);
      int getNum() const;
      int getDenom() const;
      Rational operator=(const Rational &rat);
      operator double() const;
      double getVal() const;

    private:
      int num;
      int denom;
      double val;
};


    Rational operator+(const Rational &rat1, const Rational &rat2);
    Rational operator-(const Rational &rat1, const Rational &rat2);
    Rational operator*(const Rational &rat1, const Rational &rat2);
    Rational operator/(const Rational &rat1, const Rational &rat2);
    bool operator==(const Rational &rat1, const Rational &rat2);
    bool operator!=(const Rational &rat1, const Rational &rat2);

#endif // RATIONAL_H
