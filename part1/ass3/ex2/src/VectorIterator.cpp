#include "Iterators.h"


		VectorIterator::VectorIterator(std::vector<double> vector){
				Vector = vector;
				it = Vector.begin();
		}

		double* VectorIterator::next(){
			if(it != Vector.end())
				return &(*it++);
			else return nullptr;
		}

		double VectorIterator::getMember(int elem){
			if(this->getSize() != 0)
				return Vector.at(elem);
			else return -1;
		}

		int VectorIterator::getSize() const{
			return (int)Vector.size();
		}
