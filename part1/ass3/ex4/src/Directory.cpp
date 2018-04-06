#include "FileSystemObject.h"
#include<iostream>
#include<string>


		Directory::Directory (std::string dirname) : name(dirname){}

		void Directory::addChild(std::unique_ptr<FileSystemObject> ptr){
			vectorOfPointers.push_back(std::move(ptr));
		}

		void Directory::printInfo(std::ostream& os, int a){
				os << name << "/" << std::endl;
				for (int i = 0; i < vectorOfPointers.size(); i++){
					 	for (int j = 0; j < a; j++) {
					 			os << "   ";
					 	}
						vectorOfPointers.at(i).get()->printInfo(os, a+1);
				}
		}

		void Directory::printInfo(std::ostream& os){
				printInfo(os, 1);
		}
