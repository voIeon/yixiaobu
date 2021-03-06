#pragma once

#include <vector>

#define GRID_INTERVAL_LL 1
#define GRID_INTERVAL_M	50


#define WGS84_A     6378138.0
#define WGS84_B     6356752.314245

#define WGS84_f     1/298.257223565
#define WGS84_E2    0.0066943799900965025

#define DEGTORAD(a) (a)*0.0174532925
#define RADTODEG(a) (a)*57.295779513082
#define PI          3.1415926535898

template<typename T>
struct CTVector3
{
	CTVector3()
	{
	};

	CTVector3(T _x, T _y, T _z)
		: x(_x)
		, y(_y)
		, z(_z)
	{};

	CTVector3& operator+=(CTVector3& ref)
	{
		this->x += ref.x;
		this->y += ref.y;
		this->z += ref.z;

		return (*this);
	}

	T x;
	T y;
	T z;
};

typedef CTVector3<double> CTVector3d;

typedef std::vector<CTVector3d> LLPolygon;		//点集构成区域

struct FourBoundaries
{
	FourBoundaries(float fl,float ft,float fr,float fb)
		:l(fl)
		,t(ft)
		,r(fr)
		,b(fb)
	{

	}

	bool IsCoincidence(FourBoundaries& fb)
	{
		if (this->l > fb.r ||
			this->r < fb.l ||
			this->t < fb.b ||
			this->b > fb.t)
			return false;
		else
			return true;
	}
	float l;	//left左
	float t;	//top
	float r;	//right
	float b;	//buttom
};

struct PolygonInfo
{
	LLPolygon		polygon;
	FourBoundaries	fourBound;	//shp数据中有边界数据，在网上没找到
};

typedef std::vector< PolygonInfo> Polygons;		//区域信息集合（河流集合与湖泊集合）