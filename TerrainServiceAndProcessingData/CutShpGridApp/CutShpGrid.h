#pragma once

#include <TranformationDll/GridDefine.h>

#include <QtWidgets/QWidget>
#include "ui_CutShpGrid.h"

struct CutShpGridPrivate;

class CutShpGrid : public QWidget
{
    Q_OBJECT

public:
    CutShpGrid(QWidget *parent = Q_NULLPTR);

private:

    //�зֿ�
    void CutBlock();

    //�з�դ��
    //left longitude ,buttom latitude degree
    void CutGrid(int left, int buttom);
    
    //ͨ��С���������Χ ͨ�����������ȡ��Ӧ�����ˮϵ����
    int GetPercent(FourBoundaries& fb);

//    int GetRiversPercent(double l, double t, double r, double b);
    double CalcPolygonArea(QPolygonF& polygon);
private slots:
    //����
    void CalcSlot();
private:
    Ui::CutShpGridClass ui;
    CutShpGridPrivate* _p;
};
