#include "namePairs.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>


std::ostream& operator<<(std::ostream& ostream, NamePairs& namePairs){
	std::multimap<std::string, double>::iterator iter;
	std::multimap<std::string, double> printMap = namePairs.getNamePairs();
	std::string retStr;

	//print the pair in the same form (as in excercise 2)
	for (iter = printMap.begin(); iter != printMap.end(); iter++) {
		 retStr.append("(");
		 retStr.append(iter->first);
		 retStr.append(", ");
		 std::stringstream printStream;
		 double formatDouble = iter->second;
		 printStream << formatDouble;
		 std::string printDouble = printStream.str();
		 retStr.append(printDouble);
		 retStr.append(")\n");
	}
	ostream << retStr;
	return ostream;
}

void NamePairs::setPair(std::string name,double age){
		namePairs.insert(std::pair<std::string,double>(name,age));
}

std::multimap<std::string, double> NamePairs::getNamePairs() const{
		return namePairs;
}
