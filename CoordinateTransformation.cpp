// CoordinateTransformation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "Transformation.h"

int main()
{
    CTVector3d ecef;
    Transformation::LLtoECEF(45, 45, ecef);
    double lat, lon;
    Transformation::ECEFtoLL(ecef, lat, lon);

    std::cout << "Hello World!\n";
}

