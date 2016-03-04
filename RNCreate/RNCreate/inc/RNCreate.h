#pragma once
#include "DataAcc.h"
#include "CoordTrans.h"

//#include "CSimLine.h"

//#define MIN_ZOOM		10
//#define MAX_ZOOM		16
#define MIN_ZOOM		10
#define MAX_ZOOM		16

#define MIN_COORDSPAN	5e-5	// ����������С���
#define IS_SAME_POINT(x1,y1,x2,y2)	(fabs((x1)-(x2))<MIN_COORDSPAN && fabs((y1)-(y2))<MIN_COORDSPAN)

// 18-> 28
#define FONT_WIDTH		28 


// ·���Զ�����
int AutoLabelRoadName(int a_iZoomLevel, double a_dCenterX, vector<ST_ROADLINE> &a_vecRoadLine, 
	vector<ST_ROADNAME> &a_vecRoadName,double& s_dFontLonSpan, 	double& s_dFontLatSpan);

// �����߶ηֲ���·����
int CreateRoadName(
	int				a_iZoomLevel,			// [in ] ��ǰ�����ȼ�
	vector<CRNRect>	&a_vecRect,				// [in ] ��ռ��Χ
	wstring			a_wstrName,				// [in ] ·��
	vector<double>	a_vecNameLine,			// [in ] ·����״��
	double			a_dPreNameDist,			// [in ] ���·������ʼλ��(����)
	double			a_dNameDispLen,			// [in ] ·������ʾ����
	PST_ROADNAME	a_pRoadName,		// [out] ����·�����ּ���
	double& s_dFontLonSpan, 
	double& s_dFontLatSpan);	

// ȡ��ָ��������߶��ϵĵ�
int GetPointOnLineByDistance(vector<double> vecPoints, double a_dDistance, ST_ROADNAME &a_stRoadName);

// ȡ��ָ��������߶���ʼ�������ţ�-1��ʾ�����ܳ���С��ָ�����룩
int GetPointIndexByDistance(
	vector<double>	a_vecPoints,				// [in ] ����
	double			a_dDistance,				// [in ] ָ�����루�ӵ���ʼ�����㣩
	double			&a_dOutLineDist,			// [out] ��ͷ�ۼƵ����ص�ľ���֮��
	int				a_lStartIndex = 0,			// [in ] ��ʼ������
	double			a_dStartDist = 0);			// [in ] ��ʼ����	


// ·��ƫ�� ����·����
int CreateRoadNameLine(int iZoom, double dCenterX, PST_ROADLINE pstLine, vector<double> &vecPoints);

// ��ת·������˳��
void ReserveName(PST_ROADNAME pstName, int iNameLen);


// ������о���
double GetDistance(vector<double> vecPoints);
// �������س��� �����׳���
double GetDistanceByPixel(int zoom, double lon, double lat, int len);

// �������س��� ���㾭�ȿ��
double GetLonSpanByPixel(int zoom, double lon, double lat, int len);
// �������س��� ����γ�ȿ��
double GetLatSpanByPixel(int zoom, double lon, double lat, int len);