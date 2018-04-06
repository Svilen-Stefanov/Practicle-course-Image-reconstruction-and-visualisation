#include "Rational.h"
#include <iostream>

	Rational::Rational(){}

	Rational::Rational(int numerator, int denominator){
			if(denominator == 0){
				throw std::logic_error("You can't devide by 0!");
			} else {
				num = numerator;
				denom = denominator;
				double doubleNum = static_cast<double>(num);
				double doubleDenom = static_cast<double>(denom);
			  val = doubleNum / doubleDenom;		//used for printing its value as a double
			}
	}

	Rational::operator double() const{return val;}

	Rational Rational::operator=(const Rational &rat){
			this->setNum(rat.getNum());
			this->setDenom(rat.getDenom());
			return *this;
	}

	Rational operator+(const Rational &rat1, const Rational &rat2){
		int newNum, newDenom;
		newNum = (rat1.getNum() * rat2.getDenom()) + (rat1.getDenom() * rat2.getNum());
		newDenom = rat1.getDenom() * rat2.getDenom();
		return Rational(newNum, newDenom);
	}

	Rational operator-(const Rational &rat1, const Rational &rat2){
		int newNum, newDenom;
		newNum = (rat1.getNum() * rat2.getDenom()) - (rat1.getDenom() * rat2.getNum());
		newDenom = rat1.getDenom() * rat2.getDenom();
		return Rational(newNum, newDenom);
	}

	Rational operator*(const Rational &rat1, const Rational &rat2){
		  int newNum, newDenom;
			newNum = rat1.getNum() * rat2.getNum();
			newDenom = rat1.getDenom() * rat2.getDenom();
			return Rational(newNum, newDenom);
	}

	Rational operator/(const Rational &rat1, const Rational &rat2){
		int newNum, newDenom;
		//when the denominator (rat2) = 0
		if(rat2.getNum() == 0){
			throw std::logic_error("You can't devide by 0!");
		}
		//the denominators of rat1 and rat2 are handled while creating the Rational object
		newNum = rat1.getNum() * rat2.getDenom();
					newDenom = rat1.getDenom() * rat2.getNum();
		return Rational(newNum, newDenom);

	}

	bool operator==(const Rational &rat1, const Rational &rat2){
			if(rat1.getNum() * rat2.getDenom() == rat1.getDenom() * rat2.getNum()){
					return true;
			} else return false;
	}

	bool operator!=(const Rational &rat1, const Rational &rat2){
		if(rat1.getNum() * rat2.getDenom() == rat1.getDenom() * rat2.getNum()){
				return false;
		} else return true;
	}

	void Rational::setNum(int numerator){
		num = numerator;
	}

	void Rational::setDenom(int denominator){
		denom = denominator;
	}

	int Rational::getNum() const{
		return num;
	}

	int Rational::getDenom() const{
		return denom;
	}

	double Rational::getVal() const{
	  return val;
	}
