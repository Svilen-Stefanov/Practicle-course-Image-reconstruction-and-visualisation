/*

Image Processing and Visualization using C++
Winter Semester 2016
Assignment 1

Chair for Computer Aided Medical Procedures, TUM

*/

#include <sign.h>
#include <iostream>

int main() {
	int s = sign(-5);
	
	if(s == -1)
		std::cout << "The number is negative." << std::endl;
	else if(s == 1)
		std::cout << "The number is positive." << std::endl;
	else
		std::cout << "The number is zero." << std::endl;
}
