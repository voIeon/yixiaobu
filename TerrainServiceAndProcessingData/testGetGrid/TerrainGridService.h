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

		//��ʼ��
		void Init();

		//��ȡ��������
		void ReadTerrainData();

		//��ȡʹ�õ��η���ĵ�ͼ����
		void ReadMapRegionConfig();

		//��ȡ����դ��һ�飩����������
		void ReadTerrainGridIndexDataFile(int lon, int lat);

		//��ȡ����դ��һ�飩����
		void ReadTerrainGridDataFile(int lon, int lat);

		//ͨ����γ�Ȼ�ȡ��������
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


