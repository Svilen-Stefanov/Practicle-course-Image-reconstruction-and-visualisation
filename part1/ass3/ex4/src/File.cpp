#include "FileSystemObject.h"
#include<iostream>
#include<string>

	File::File (std::string filename) : name(filename){}

	void File::printInfo(std::ostream& os){
			os << name  << std::endl;
	}

	void File::printInfo(std::ostream& os, int a){
			printInfo(os);
	}
