/*

Image Processing and Visualization using C++
Winter Semester 2016
Assignment 1

Chair for Computer Aided Medical Procedures, TUM

*/

#include "sign.h"

/**
 * Implements the signum function https://en.wikipedia.org/wiki/Sign_function
 */
int sign(int number) {
	if (number == 0)
		return 0;
	else if (number > 0)
		return 1;
	else return -1;
}
