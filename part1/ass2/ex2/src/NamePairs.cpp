#include "NamePairs.h"
#include <iostream>
#include <string>
#include <sstream>


std::ostream& operator<<(std::ostream& ostream, NamePairs& namePairs){
		if(namePairs.getNames().size() != namePairs.getAges().size()){
				throw std::logic_error("Mismatch of the number of names and ages in the list!");
		}

			for (unsigned i = 0; i < namePairs.getNames().size(); i++){
					//Fortmat the double value of Ages
					std::stringstream printStream;
					double formatDouble = namePairs.getAges().at(i);
					printStream << formatDouble;
					std::string printDouble = printStream.str();
					//print the Pair in form -> (Name, Age)
					ostream << "(" + namePairs.getNames().at(i) + ", " + printDouble + ")\n";
			}
			return ostream;
}

void NamePairs::setNames(std::vector<std::string> names){
    Names = names;
}

void NamePairs::setAges(std::vector<double> ages){
    Ages = ages;
}

std::vector<std::string> NamePairs::getNames() const{
		return Names;
}
std::vector<double> NamePairs::getAges() const{
		return Ages;
}

void NamePairs::sort(){
			if(this->Names.size() != this->Ages.size()){
				  throw std::logic_error("Mismatch of the number of names and ages in the list!");
			}

			//chosen because of implementation easiness -> Merge- or Quicksort will be faster and more reasonable
	    for(unsigned i = 1; i < this->Names.size(); i++){
	        for(int j = i; j > 0; j--){
	            if((this->Names.at(j).compare(this->Names.at(j-1))) < 0){
	                std::swap(this->Names.at(j), this->Names.at(j-1));
	                std::swap(this->Ages.at(j), this->Ages.at(j-1));
	            }
	        }
	    }
}
