#ifndef ITERATORS_H
#define ITERATORS_H
#include<vector>
#include<list>
#include<iostream>
#include <string>
#include <sstream>

class Iterator {
    public:
      virtual double* next() = 0;
      virtual double getMember(int) = 0;
      virtual int getSize() const = 0;
};

class VectorIterator : public Iterator {
    public:
      VectorIterator(std::vector<double>);
      double* next();
      double getMember(int);
      int getSize() const;

    private:
      std::vector<double> Vector;
      std::vector<double>::iterator it;
};

class ListIterator : public Iterator {
    public:
      ListIterator(std::list<double>);
      double* next();
      double getMember(int);
      int getSize() const;

    private:
      std::list<double> List;
      std::list<double>::iterator it;
};

inline void print(Iterator& iterator, std::ostream& ostream){
    for (int i = 0; i < iterator.getSize(); i++) {
        std::stringstream printStream;
        double formatDouble = iterator.getMember(i);
        printStream << formatDouble;
        std::string printDouble = printStream.str();
        ostream << printDouble << std::endl;
      }
}

#endif // ITERATORS_H
