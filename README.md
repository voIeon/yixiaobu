# yixiaobu
### **shp矢量水系地图切分栅格**

​	读取shp水系地图中河流湖泊的数据，根据需求计算需求区域（四至范围）与水系的栅格数据。栅格的边长为5-50m。需要计算每个小格子与水系的交集，计算出交集的面积占方格面积的比值，作为栅格的中的水系数据

包含功能：

​	**坐标系转换** coordinate system transformation

​		1.ECEF坐标与LLA坐标的转换

​		2.ECEF坐标在ENU坐标的旋转矩阵

​		3.基于旋转矩阵计算对应坐标点（未完全展开矩阵乘法公式，已展开的能够完成计算）

​	**栅格切分** 

​		分两步处理栅格，第一步对经纬度区域进行预处理，分成n个以1度（可以为0.5度）为边长（度为单位）的块。

​		对块进行栅格处理，以50m为边长，切割成更小的格子

​	**小栅格与shp数据交集计算**

​		将格子与水系数据中的区域进行交集计算，计算交集区域的面积，（计算面积公式比较长，之后录入）计算出水系占格子的比例。

​		计算区域面积，需要将LL转换为ecef，进行矢量计算。

​	**使用**

​		定位栅格，给出一个坐标，快速定位到所在栅格，获取当前坐标位置的水系。

局限性：

​	矢量数据是存储的是polygon(5)类型  polygon可以相对准确的描述水域的特征，本算法对存储类型有特殊要求，就是较大或复杂的水域需要处理为多块。目前没有在网上找到这种类型的shp地图。导致无法测试完整流程。

​	目前处理经度（0E-180E） 纬度（0N-90N）区域



