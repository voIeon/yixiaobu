#pragma once

#include "GridDefine.h"

#include <QtWidgets/QWidget>
#include "ui_CutShpGrid.h"

struct CutShpGridPrivate;

class CutShpGrid : public QWidget
{
    Q_OBJECT

public:
    CutShpGrid(QWidget *parent = Q_NULLPTR);

private:

    //切分块
    void CutBlock();

    //切分栅格
    //left longitude ,buttom latitude degree
    void CutGrid(int left, int buttom);
    
    //通过小格的四至范围 通过交集计算获取对应区域的水系比例
    int GetPercent(FourBoundaries& fb);

//    int GetRiversPercent(double l, double t, double r, double b);
    double CalcPolygonArea(QPolygonF& polygon);
private slots:
    //计算
    void CalcSlot();
private:
    Ui::CutShpGridClass ui;
    CutShpGridPrivate* _p;
};
