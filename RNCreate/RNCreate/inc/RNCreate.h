#pragma once
#include "DataAcc.h"
#include "CoordTrans.h"

//#include "CSimLine.h"

//#define MIN_ZOOM		10
//#define MAX_ZOOM		16
#define MIN_ZOOM		10
#define MAX_ZOOM		16

#define MIN_COORDSPAN	5e-5	// 共点坐标最小误差
#define IS_SAME_POINT(x1,y1,x2,y2)	(fabs((x1)-(x2))<MIN_COORDSPAN && fabs((y1)-(y2))<MIN_COORDSPAN)

// 18-> 28
#define FONT_WIDTH		28 


// 路名自动放置
int AutoLabelRoadName(int a_iZoomLevel, double a_dCenterX, vector<ST_ROADLINE> &a_vecRoadLine, 
	vector<ST_ROADNAME> &a_vecRoadName,double& s_dFontLonSpan, 	double& s_dFontLatSpan);

// 根据线段分布道路名称
int CreateRoadName(
	int				a_iZoomLevel,			// [in ] 当前比例等级
	vector<CRNRect>	&a_vecRect,				// [in ] 已占范围
	wstring			a_wstrName,				// [in ] 路名
	vector<double>	a_vecNameLine,			// [in ] 路名形状线
	double			a_dPreNameDist,			// [in ] 输出路名的起始位置(距离)
	double			a_dNameDispLen,			// [in ] 路名的显示长度
	PST_ROADNAME	a_pRoadName,		// [out] 单条路线文字集合
	double& s_dFontLonSpan, 
	double& s_dFontLatSpan);	

// 取得指定距离的线段上的点
int GetPointOnLineByDistance(vector<double> vecPoints, double a_dDistance, ST_ROADNAME &a_stRoadName);

// 取得指定距离的线段起始点索引号（-1表示折线总长度小于指定距离）
int GetPointIndexByDistance(
	vector<double>	a_vecPoints,				// [in ] 点列
	double			a_dDistance,				// [in ] 指定距离（从点列始点起算）
	double			&a_dOutLineDist,			// [out] 从头累计到返回点的距离之和
	int				a_lStartIndex = 0,			// [in ] 起始点索引
	double			a_dStartDist = 0);			// [in ] 起始距离	


// 路线偏移 生成路名线
int CreateRoadNameLine(int iZoom, double dCenterX, PST_ROADLINE pstLine, vector<double> &vecPoints);

// 倒转路名文字顺序
void ReserveName(PST_ROADNAME pstName, int iNameLen);


// 计算点列距离
double GetDistance(vector<double> vecPoints);
// 根据像素长度 计算米长度
double GetDistanceByPixel(int zoom, double lon, double lat, int len);

// 根据像素长度 计算经度跨度
double GetLonSpanByPixel(int zoom, double lon, double lat, int len);
// 根据像素长度 计算纬度跨度
double GetLatSpanByPixel(int zoom, double lon, double lat, int len);