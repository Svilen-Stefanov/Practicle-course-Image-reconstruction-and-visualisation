#ifndef TRACER_H
#define TRACER_H
#include<iostream>

class Tracer
{
    public:
        Tracer();
        Tracer(std::string);
        Tracer(const Tracer&);       //copy constructor
        Tracer& operator=(const Tracer&);     // copy assignment operator
        Tracer(Tracer&&);             // move constructor
        Tracer& operator=(Tracer&&);    // move assignment operator
        ~Tracer();
        std::string string;
};


#endif // TRACER_H
