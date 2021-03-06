#include "CutShpGrid.h"

//#include "GridDefine.h"

#include "Transformation.h"

struct CutShpGridPrivate
{
	int		 left;
	int		 top;
	int		 right;
	int      buttom;
	int		 interval;

	Polygons polygonsRivers;
	Polygons polygonsLakes;
};

CutShpGrid::CutShpGrid(QWidget *parent)
    : QWidget(parent)
	, _p(new CutShpGridPrivate)
{
    ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(CalcSlot()));
}

//大地图切块
void CutShpGrid::CutBlock()
{
	int il = _p->left;
	const int it = _p->top;
	const int ir = _p->right;
	int ib = _p->buttom;

	for (; il < ir; ++il)
	{
		for (; ib < it; ++ib)
		{
			//考虑使用多线程
			CutGrid(il,ib);
		}
	}
}

//栅格处理
void CutShpGrid::CutGrid(int left, int buttom)
{
	//四至范围
	const double l = (double)left;
	const double b = (double)buttom;
	const double r = l + GRID_INTERVAL_LL;
	const double t = b + GRID_INTERVAL_LL;
	
	const int interval = _p->interval;
	const int intSize = sizeof(int);
	
	 //		^ ->->->
	 //		| ->->->->
	 //		| ->->->->
	 //		^ ->->->->->
	 //		| ->->->->->->
	//		| ->->->->->->
	 //	  17^ ->->->->->->->
	 //	  10| ->->->->->->->->
	//		  9 A B C D E F 10
	//	   8| ->->->->->->->->
	//		  1  2 3 4 5 6 7 8
	//保存两个二进制文件
	//例如 100_25 和100_25.bin 保存左下角坐标（100，25）的边长为1度的块
	//栅格从左下角位置由左向右增加，到达经度最大值，返回最左侧，纬度再增加，经度再重复上两步。
	//100_25 蛇形向上，索引增加，与所在栅格的水系比例构成一条数据。在1度的块中大概有400w组数据，
	//	长度为4,000,000 * sizeof(int) * 2 36m
	//100_25.bin 记录蛇形向上每一行最后一个值，用来快速索引

	QString gridFileName = QString::number(l) + "_" + QString::number(b);
	QString indexFileName = gridFileName + ".bin";
	FILE* fpGrid;
	FILE* fpIndex;
	fpGrid = fopen(gridFileName.toStdString().c_str(), "wb+");
	fpIndex = fopen(indexFileName.toStdString().c_str(), "wb+");

	CTVector3d ecef;
	//lat在前 lon在后
	CTVector3d stepX1(0, interval, 0);
	CTVector3d stepY1(interval, 0, 0);
	CTVector3d stepY0;
	CTVector3d stepX0;
	double rot[9] = {};

	int i = 0;
	double temLat = b;
	//纬度递增
	while (temLat < t)
	{
		double temLon = l;
		double temLat2;
		Transformation::LLtoECEF(temLat, temLon, ecef);
		Transformation::ECEFtoENURotateMatrix(ecef, rot);
		Transformation::Rotate10(rot, stepX1, stepX0);
		ecef += stepX0;
		double lonDeg;
		double latDeg;
		Transformation::ECEFtoLL(ecef, latDeg, lonDeg);
		temLat2 = latDeg;

		double lat, lon;
		//经度递增
		while (temLon < r)
		{
			Transformation::LLtoECEF(b, temLon, ecef);
			Transformation::ECEFtoENURotateMatrix(ecef, rot);
			Transformation::Rotate10(rot, stepY1, stepY0);
			ecef += stepY0;
			Transformation::ECEFtoLL(ecef, lat, lon);

			int iPercent = GetPercent(FourBoundaries(temLon,temLat2,lon,temLat));
			
			i++;
			temLon = lon;
			
			fwrite(&i, intSize, 1, fpGrid);
			fwrite(&iPercent, intSize, 1, fpGrid);
			//四至 temLon temLat2 lon temLat
		}
		fflush(fpGrid);
		
		fwrite(&i, intSize, 1, fpIndex);
		fflush(fpIndex);

		temLat = temLat2;
	}
	fclose(fpGrid);
	fclose(fpIndex);
}

//获取水系比例
int CutShpGrid::GetPercent(FourBoundaries & fb)
{
	double dPercent = 0.0;

	//河流
	const Polygons& rivers = _p->polygonsRivers;
	for (auto river : rivers)
	{
		//预处理 过滤数据 帅选出四至范围有重叠的进行交集区域计算
		if (fb.IsCoincidence(river.fourBound))
		{
			QPolygonF gridPolygon;
			gridPolygon << QPointF(fb.l, fb.b)
				<< QPointF(fb.l, fb.t) << QPointF(fb.r, fb.t) << QPointF(fb.r, fb.b);

			QPolygonF riverPolygon;

			QPolygonF intersectPolygon = riverPolygon.intersected(gridPolygon);
			double s = CalcPolygonArea(intersectPolygon);
			//计算比例 存在1-2个河流湖泊与栅格交集 
			double ss = ss / (_p->interval * _p->interval) * 100.0;
			if (s > 0)
			{
				dPercent += ss;
			}
		}
	}

	//湖泊
	const Polygons& lakes = _p->polygonsLakes;
	for (auto lake : lakes)
	{

	}

	return (int)dPercent;
}

//计算区域面积 较长
double CutShpGrid::CalcPolygonArea(QPolygonF& polygon)
{
	return 0.0;
}

void CutShpGrid::CalcSlot()
{
	_p->left = ui.spinBox_l->value();
	_p->top = ui.spinBox_t->value();
	_p->right = ui.spinBox_r->value();
	_p->buttom = ui.spinBox_b->value();

	_p->interval = ui.spinBox_interval->value();

	//大地图切块
	CutBlock();
}