#include "TerrainGridService.h"

#include <string>
#include <f:/workspace/CutShpGrid/CutShpGrid/Transformation.h>

#include <f:/workspace/CutShpGrid/CutShpGrid/GridDefine.h> //临时的

using namespace std;

namespace SHPMAP
{
	TerrainGridService::TerrainGridService()
		:_Left(0),_Top(0),_Right(0),_Buttom(0)
		,_interval(0)
	{

	}
	void TerrainGridService::Init()
	{
		ReadMapRegionConfig();
		ReadTerrainData();
	}

	void TerrainGridService::ReadTerrainData()
	{
		_Left	= 120;
		_Top	= 30;
		_Right	= 123;
		_Buttom = 28;

		_interval = 50;//m
	}

	void TerrainGridService::ReadMapRegionConfig()
	{
		unsigned int il = _Left;
		unsigned int ib = _Buttom;

		//再纵向向上 （一行）
		for (; ib < _Top; ++ib)
		{
			//先横向移动 （一列）
			for (; il < _Right; ++il)
			{
				ReadTerrainGridIndexDataFile(il, ib);
				ReadTerrainGridDataFile(il, ib);
			}
		}
	}

	void TerrainGridService::ReadTerrainGridIndexDataFile(int lon, int lat)
	{
		string strLon = to_string(lon);
		string strLat = to_string(lat);
		string fileName = strLon + "_" + strLat;

		FILE* fp;
		fp = fopen(fileName.c_str(), "rb+");
		fseek(fp, 0L, SEEK_END);
		int fileLenght = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		char* buffer = new char[fileLenght];
		memset(buffer, 0, fileLenght);
		fread(buffer, 1, fileLenght, fp);
		
		vector<GridInfo> gridInfos((GridInfo*)buffer, (GridInfo*)buffer + fileLenght / sizeof(GridInfo));

		_vecGridInfos.push_back(gridInfos);
		
		delete[] buffer;
	}

	void TerrainGridService::ReadTerrainGridDataFile(int lon, int lat)
	{
		string strLon = to_string(lon);
		string strLat = to_string(lat);
		string fileName = strLon + "_" + strLat + ".bin";

		FILE* fp;
		fp = fopen(fileName.c_str(), "rb+");
		fseek(fp, 0L, SEEK_END);
		int fileLenght = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		char* buffer = new char[fileLenght];
		memset(buffer, 0, fileLenght);
		fread(buffer, 1, fileLenght, fp);

		vector<int> indexs((int*)buffer, (int*)buffer + fileLenght / sizeof(int));

		_vecIndexs.push_back(indexs);

		delete[] buffer;
	}

	int TerrainGridService::GetPercentByLL(double lon, double lat)
	{
		if (lon < _Left || lat < _Buttom || lat > _Top || lon > _Right) return 1.0;

		int iColumn = _Right - _Left;
		int indexBlock = iColumn * (lat - _Buttom) + (lon - _Left) ;

		vector <int >& vecIndex = _vecIndexs.at(indexBlock);
		vector <GridInfo>& vecGridInfo = _vecGridInfos.at(indexBlock);

		int iMax = vecIndex.at(0);
		int jMax = vecIndex.size();

		//引用
		GetGridIndexInBlock(iMax, jMax, lon, lat);

		if (jMax > 0)
		{
			jMax--;
		}

		int gridIndex = vecIndex.at(jMax) + iMax;

		int percent = vecGridInfo.at(gridIndex).percent;
		return 1.0;
	}

	void TerrainGridService::GetGridIndexInBlock(int& iColumn, int& jRow,const double lon, const double lat)
	{
		double dLon = (int)lon;
		double dLat = (int)lat;

		CTVector3d ecef;
		//lat在前 lon在后
		CTVector3d stepX1(0, _interval, 0);
		CTVector3d stepY1(_interval, 0, 0);
		CTVector3d stepY0;
		CTVector3d stepX0;

		double rot[9];

		double lonDeg;
		double latDeg;

		for (int j = 0; j < jRow; ++j)
		{

			Transformation::LLtoECEF(dLat, dLon, ecef);
			Transformation::ECEFtoENURotateMatrix(ecef, rot);
			Transformation::Rotate10(rot, stepX1, stepX0);
			ecef += stepX0;

			Transformation::ECEFtoLL(ecef, latDeg, lonDeg);
			dLat = latDeg;

			if (dLat > lat)
			{
				for (int i = 0; i < iColumn; ++i)
				{
					Transformation::LLtoECEF(lat, dLon, ecef);
					Transformation::ECEFtoENURotateMatrix(ecef, rot);
					Transformation::Rotate10(rot, stepY1, stepY0);
					ecef += stepY0;
					Transformation::ECEFtoLL(ecef, latDeg, lonDeg);

					if (lonDeg > lon)
					{
						iColumn = i;
						jRow = j;
						break;
					}
					dLon = lonDeg;
				}
				break;
			}

		}
	}
}