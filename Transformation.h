#pragma once

//  WGS84

// ECEF -> LL
// LL   -> ECEF
// ECEF -> ENU rotate matrix
// /*--ENU  -> ECEF*/
template<typename T>
struct CTVector3
{
	CTVector3()
	{};
	CTVector3(T _x,T _y,T _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{};
	T x;
	T y;
	T z;
};

typedef CTVector3<double> CTVector3d;

class Transformation
{
public:
	static void ECEFtoLL(CTVector3d& ecef, double& lat, double& lon);

	static void LLtoECEF(double lat, double lon, CTVector3d& ecef);
};

