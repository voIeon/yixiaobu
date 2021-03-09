#pragma once

#include "GridDefine.h"

//  WGS84

// ECEF -> LL
// LL   -> ECEF
// ECEF -> ENU rotate matrix
// /*--ENU  -> ECEF*/



class TRANSFORMATION_DECL Transformation
{
public:
	static void ECEFtoLL(CTVector3d& ecef, double& lat, double& lon);

	static void LLtoECEF(double lat, double lon, CTVector3d& ecef);

	static void ECEFtoENURotateMatrix(CTVector3d& ecef, double(&Rot)[9]);
	
	//计算在坐标系C0上的地心地固位置点ecef0对应在坐标系C1上的位置点ecef1
	//	参数：Rot  C0->C1的旋转矩阵
	//	参数：ecef0 在C0上的地心地固坐标点
	//	参数：ecef1[in,out] 在C1上的坐标点
	static void Rotate01(double (&Rot)[9],CTVector3d& ecef0,CTVector3d& ecef1);
	
	//计算在坐标系C1上的地心地固位置点ecef1对应在坐标系C0上的位置点ecef0
	//	参数：Rot  C0->C1的旋转矩阵
	//	参数：ecef1 在C1上的地心地固坐标点
	//	参数：ecef10[in,out] 在C0上的坐标点
	static void Rotate10(double (&Rot)[9],CTVector3d& ecef1,CTVector3d& ecef0);
};

