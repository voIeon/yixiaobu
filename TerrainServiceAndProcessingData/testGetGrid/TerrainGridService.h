#pragma once
#include "GridInfo.h"

#include <vector>

#include <boost/serialization/singleton.hpp> 

namespace SHPMAP
{
	class TerrainGridService
	{
	public:
		TerrainGridService();
	public:

		//初始化
		void Init();

		//读取地形数据
		void ReadTerrainData();

		//读取使用地形服务的地图区域
		void ReadMapRegionConfig();

		//读取地形栅格（一块）的索引数据
		void ReadTerrainGridIndexDataFile(int lon, int lat);

		//读取地形栅格（一块）数据
		void ReadTerrainGridDataFile(int lon, int lat);

		//通过经纬度获取地形数据
		int  GetPercentByLL(double lon, double lat);

		void GetGridIndexInBlock(int& iColumn, int& jRow, const double lon, const double lat);
	private:
		int _Left;
		int _Top;
		int _Right;
		int _Buttom;

		int _interval;

		std::vector<std::vector< GridInfo> > _vecGridInfos;
		std::vector<std::vector<int> >		 _vecIndexs;
	};

	typedef boost::serialization::singleton<TerrainGridService> TerrainGridServiceSingleton;
}


