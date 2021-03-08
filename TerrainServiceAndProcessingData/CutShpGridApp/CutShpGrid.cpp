#include "CutShpGrid.h"

//#include "GridDefine.h"

#include <TranformationDll/Transformation.h>

struct CutShpGridPrivate
{
	CutShpGridPrivate()
	{
		polygonsRivers.reserve(4400000);
		polygonsLakes.reserve(4400000);
	}
	//������Χ
	int		 left;
	int		 top;
	int		 right;
	int      buttom;
	//դ�񲽳�
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

//���ͼ�п�
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
			//����ʹ�ö��߳�
			CutGrid(il,ib);
		}
	}
}

//դ����
void CutShpGrid::CutGrid(int left, int buttom)
{
	//������Χ
	const double l = (double)left;
	const double b = (double)buttom;
	const double r = l + GRID_INTERVAL_LL;
	const double t = b + GRID_INTERVAL_LL;
	
	const int interval = _p->interval;
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
	//���������������ļ�
	//���� 100_25 ��100_25.bin �������½����꣨100��25���ı߳�Ϊ1�ȵĿ�
	//դ������½�λ�������������ӣ����ﾭ�����ֵ����������࣬γ�������ӣ��������ظ���������
	//100_25 �������ϣ��������ӣ�������դ���ˮϵ��������һ�����ݡ���1�ȵĿ��д����400w�����ݣ�
	//	����Ϊ4,000,000 * sizeof(int) * 2 36m
	//100_25.bin ��¼��������ÿһ�����һ��ֵ��������������

	QString gridFileName = QString::number(l) + "_" + QString::number(b);
	QString indexFileName = gridFileName + ".bin";
	FILE* fpGrid;
	FILE* fpIndex;
	fpGrid = fopen(gridFileName.toStdString().c_str(), "wb+");
	fpIndex = fopen(indexFileName.toStdString().c_str(), "wb+");

	CTVector3d ecef;
	//lat��ǰ lon�ں�
	CTVector3d stepX1(0, interval, 0);
	CTVector3d stepY1(interval, 0, 0);
	CTVector3d stepY0;
	CTVector3d stepX0;
	double rot[9] = {};

	int i = 0;
	double temLat = b;
	//γ�ȵ���
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
		//���ȵ���
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
			double l1 = lat;
			fwrite(&i, 4, 1, fpGrid);
			fwrite(&iPercent, 4, 1, fpGrid);
			//���� temLon temLat2 lon temLat
		}
		fflush(fpGrid);
		
		fwrite(&i, 4, 1, fpIndex);
		fflush(fpIndex);

		temLat = temLat2;
	}
	fclose(fpGrid);
	fclose(fpIndex);
}

//��ȡˮϵ����
int CutShpGrid::GetPercent(FourBoundaries & fb)
{
	double dPercent = 0.0;

	//����
	const Polygons& rivers = _p->polygonsRivers;
	for (auto river : rivers)
	{
		//Ԥ���� �������� ˧ѡ��������Χ���ص��Ľ��н����������
		if (fb.IsCoincidence(river.fourBound))
		{
			QPolygonF gridPolygon;
			gridPolygon << QPointF(fb.l, fb.b)
				<< QPointF(fb.l, fb.t) << QPointF(fb.r, fb.t) << QPointF(fb.r, fb.b);

			QPolygonF riverPolygon;

			QPolygonF intersectPolygon = riverPolygon.intersected(gridPolygon);
			double s = CalcPolygonArea(intersectPolygon);
			//������� ����1-2������������դ�񽻼� 
			double ss = s / pow(_p->interval,2) * 100.0 ;
			if (ss > 0)
			{
				dPercent += ss;
			}
		}
	}

	//����
	const Polygons& lakes = _p->polygonsLakes;
	for (auto lake : lakes)
	{

	}

	return (int)dPercent;
}

//����������� �ϳ�
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

	//���ͼ�п�
	CutBlock();
}