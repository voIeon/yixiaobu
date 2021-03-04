// CoordinateTransformation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>

#include "Transformation.h"

int main()
{
    //test http://www.sysense.com/products/ecef_lla_converter/index.html
    CTVector3d ecef;
    Transformation::LLtoECEF(45, 45, ecef);
    double lat, lon;
    Transformation::ECEFtoLL(ecef, lat, lon);

    std::cout << std::setprecision(10) << lat<<" " << lon<< "Hello World!\n";
}

