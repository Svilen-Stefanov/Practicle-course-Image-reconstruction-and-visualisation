#include "Tracer.h"
#include<iostream>
#include<string>


		Tracer::Tracer(){
				string = "";
		}

		Tracer::Tracer(std::string String){
				string = String;
				std::cout << "Constructor with value: \"" << string << "\" was created!" << std::endl;
		}

		Tracer::~Tracer(){
				std::cout << "Destructor with value: \"" << string << "\" was used!" << std::endl;
		}

		Tracer::Tracer(const Tracer& tr) : string(tr.string){				 //copy constructor
				std::cout << "This is a copy constructor for: \"" << this->string << "\"" << std::endl;
		}

		Tracer& Tracer::operator=(const Tracer& tr){					 //copy assignment operator
				string = tr.string;
				std::cout << "This is a copy assignment constructor for: \"" << this->string << "\"" << std::endl;
				return *this;
		}

		Tracer::Tracer(Tracer&& tr) : string(std::move(tr.string)){				//move constructor
				std::cout << "This is a move constructor for: \"" << this->string << "\"" << std::endl;
		}

		Tracer& Tracer::operator=(Tracer&& tr) {					//move assignment operator
				string = std::move(tr.string);
				std::cout << "This is a move assignment constructor for: \"" << this->string << "\"" << std::endl;
				return *this;
		}

		Tracer testMember ("Member object");
