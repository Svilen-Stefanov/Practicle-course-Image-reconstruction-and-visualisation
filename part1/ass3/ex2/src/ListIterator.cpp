#include "Iterators.h"


		ListIterator::ListIterator(std::list<double> list){
				List = list;
				it = List.begin();
		}

		double* ListIterator::next(){
			if(it != List.end())
				return &(*it++);
			else return nullptr;
		}

		double ListIterator::getMember(int elem){
			auto element = List.begin();
			if(this->getSize() != 0)
				std::advance(element, elem);
			else *element = -1;

			return *element;
		}

		int ListIterator::getSize() const{
				return (int)List.size();
		}
