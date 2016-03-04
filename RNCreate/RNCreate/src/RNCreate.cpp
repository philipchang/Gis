#include "CRNCreate.h"


int main(int argc,char **argv)
{
	CRNCreate rnCreator;
	rnCreator.ProcessRN();
		
	printf("��·���Ʊ������\n");
	system("pause");
}
/*
int test()
{
	char *pcBasePath = "./srcMap";//argv[1];
	double dCenterX = 0;//atof(argv[1]);
	double dCenterY = 0;//atof(argv[2]);

	ReadCentXYFile( "./srcMap/CenterXY.csv",dCenterX, dCenterY);

	char pMifName[256] = {0};
	char pMidName[256] = {0};

	sprintf_s(pMifName, 256, "%s/lines_test.MIF", pcBasePath);
	sprintf_s(pMidName, 256, "%s/lines_test.MID", pcBasePath);
	vector<ST_ROADLINE> vecRoadLine = ReadMifMidFile(pMifName, pMidName);

	if (vecRoadLine.size() <= 0) {
		return RET_FAILED;
	}

	// merge same road
	vector<ST_ROADLINE> vecDesRoadLine;
	CMergeRoader::MergeRoad(vecRoadLine,vecDesRoadLine);
	_mkdir("srcMap/RoadName");

	for (int iZoom = MIN_ZOOM; iZoom <= MAX_ZOOM; iZoom++)
	{
		// proces 10,12,14,16
		if(iZoom % 2 != 0)
			continue;

		// ����һ�����ֵľ�γ�ȿ��
		s_dFontLonSpan = GetLonSpanByPixel(iZoom, dCenterX, dCenterY, FONT_WIDTH);
		s_dFontLatSpan = GetLatSpanByPixel(iZoom, dCenterX, dCenterY, FONT_WIDTH);

		vector<ST_ROADNAME> vecRoadName;
		AutoLabelRoadName(iZoom, dCenterX, vecDesRoadLine, vecRoadName);

		sprintf_s(pMifName, 256, "%s/RoadName/RoadName_%d.MIF", pcBasePath, iZoom);
		sprintf_s(pMidName, 256, "%s/RoadName/RoadName_%d.MID", pcBasePath, iZoom);
		OutputRoadName(pMifName, pMidName, vecRoadName);
	}

// 	FILE *pf = fopen("zoomList.txt", "w");
// 	for (int i = MIN_ZOOM; i <= MAX_ZOOM; i++)
// 	{
// 		double len = GetDistanceByPixel(i, 119.4, 39.9, 1);
// 		fprintf(pf, "zoom[%d]:%f\n", i, len);
// 	}
	
	printf("��·���Ʊ������\n");
	system("pause");
	return RET_SUCCESS;
}

*/
/************************************************************************/
/* ��·�����Ų�                                                         */
/************************************************************************/

// ·���Զ�����
int AutoLabelRoadName(int a_iZoomLevel, double a_dCenterX, vector<ST_ROADLINE> &a_vecRoadLine,
	vector<ST_ROADNAME> &a_vecRoadName,double& s_dFontLonSpan, 	double& s_dFontLatSpan)
{
	if (a_vecRoadLine.size() <= 0) {
		return RET_SUCCESS;
	}

	double	dTotalDist = 0.0;			// ·�߳���
	double	dPreNameDist = 0.0;			// ·����ʼ����
	double	dNameDispLen = 0.0;			// ·����ʾ����

	vector<CRNRect> vecRect;			// ·�����ط�Χ

	// ����ȫ����·
	vector<ST_ROADLINE>::iterator iterLine = a_vecRoadLine.begin();
	for (; iterLine != a_vecRoadLine.end(); iterLine++)
	{
		if (iterLine->flag != 0) {
			continue;
		}

		PST_ROADLINE pstLine = &(*iterLine);
		double dX0 = pstLine->vecPoints[0];
		double dY0 = pstLine->vecPoints[1];
		double dX1 = pstLine->vecPoints[pstLine->vecPoints.size()-2];
		double dY1 = pstLine->vecPoints[pstLine->vecPoints.size()-1];

		pstLine->flag = 1;

		// �������յ���ͬ��·
		vector<ST_ROADLINE>::iterator iterLineTmp = a_vecRoadLine.begin();
		for (; iterLineTmp != a_vecRoadLine.end(); iterLineTmp++)
		{
			if (iterLineTmp->flag != 0) {
				continue;
			}

			PST_ROADLINE pstLineTmp = &(*iterLineTmp);
			double dX3 = pstLineTmp->vecPoints[0];
			double dY3 = pstLineTmp->vecPoints[1];
			double dX4 = pstLineTmp->vecPoints[pstLineTmp->vecPoints.size()-2];
			double dY4 = pstLineTmp->vecPoints[pstLineTmp->vecPoints.size()-1];

			if ((IS_SAME_POINT(dX0,dY0,dX3,dY3) && IS_SAME_POINT(dX1,dY1,dX4,dY4)) ||  
				(IS_SAME_POINT(dX1,dY1,dX3,dY3) && IS_SAME_POINT(dX0,dY0,dX4,dY4)))
			{
				pstLineTmp->flag = 1;
				if (pstLineTmp->iShowLev < pstLine->iShowLev) {
					pstLine = pstLineTmp;
				}
				// gaojian test iDrawLev ���� ��������
				else if (pstLineTmp->iDrawLev < pstLineTmp->iDrawLev)
				{
					pstLine = pstLineTmp;
				}
			}
		}


		// ��ʼ����·��

		// ·��ƫ�� ����·����
		vector<double> vecNameLine;
		CreateRoadNameLine(a_iZoomLevel, a_dCenterX, pstLine, vecNameLine);

		// ·����������
		int iNameLen = pstLine->wstrRoadName.size();
		if (iNameLen <= 0) {
			continue;
		}

		// �����߶ξ���(MKT �׵�λ)
		dTotalDist = GetDistance(vecNameLine);
		if (dTotalDist <= 0) {
			continue;
		}

		// �������ֳ���
		int iNameDispLenPx = FONT_WIDTH * iNameLen;
		dNameDispLen = GetDistanceByPixel(a_iZoomLevel, vecNameLine[0], vecNameLine[1], iNameDispLenPx);

		dPreNameDist = (dTotalDist - dNameDispLen) / 2;
		if (dPreNameDist < 0 || dPreNameDist >= dTotalDist) {
			continue;
		}

		// ���ɵ�·����
		if (a_iZoomLevel >= pstLine->iShowLev) 
		{
			PST_ROADNAME pstRoadName = new ST_ROADNAME[iNameLen];
			int iRet = CreateRoadName(a_iZoomLevel, vecRect, pstLine->wstrRoadName, vecNameLine,
				dPreNameDist, dNameDispLen, pstRoadName, s_dFontLonSpan, s_dFontLatSpan);
			if (iRet == RET_SUCCESS) {
				for (int i = 0; i < iNameLen; i++) {
					a_vecRoadName.push_back(pstRoadName[i]);
				}
			} else {
				//printf("������[%d] ����·�� ����\n", a_iZoomLevel);
			}
			delete pstRoadName;
		}
	}

	// �ָ����
	iterLine = a_vecRoadLine.begin();
	for (; iterLine != a_vecRoadLine.end(); iterLine++)
	{
		iterLine->flag = 0;
	}


// 	// ����ȫ����·
// 	vector<ST_ROADLINE>::iterator iterLine = a_vecRoadLine.begin();
// 	for (; iterLine != a_vecRoadLine.end(); iterLine++)
// 	{
// 		ST_ROADLINE &stLine = *iterLine;
// 
// 		// ·����������
// 		int iNameLen = stLine.wstrRoadName.size();
// 		if (iNameLen <= 0) {
// 			continue;
// 		}
// 
// 		// �����߶ξ���(MKT �׵�λ)
// 		dTotalDist = GetDistance(stLine.vecPoints);
// 		if (dTotalDist <= 0) {
// 			continue;
// 		}
// 
// 		// �������ֳ���
// 		int iNameDispLenPx = FONT_WIDTH * iNameLen;
// 		dNameDispLen = GetDistanceByPixel(a_iZoomLevel, stLine.vecPoints[0], stLine.vecPoints[1], iNameDispLenPx);
// 
// 		dPreNameDist = (dTotalDist - dNameDispLen) / 2;
// 		if (dPreNameDist < 0 || dPreNameDist >= dTotalDist) {
// 			return RET_SUCCESS;
// 		}
// 
// 		// ���ɵ�·����
// 		if (a_iZoomLevel >= stLine.iShowLev) 
// 		{
// 			PST_ROADNAME pstRoadName = new ST_ROADNAME[iNameLen];
// 			int iRet = CreateRoadName(a_iZoomLevel, stLine, dPreNameDist, dNameDispLen, pstRoadName);
// 			if (iRet == RET_SUCCESS) {
// 				for (int i = 0; i < iNameLen; i++)
// 				{
// 					a_vecRoadName.push_back(pstRoadName[i]);
// 				}
// 			} else {
// 				printf("������[%d] ����·�� ����\n", a_iZoomLevel);
// 			}
// 		}
// 	}

	return RET_SUCCESS;
}

// �����߶ηֲ���·����
int CreateRoadName(
	int				a_iZoomLevel,			// [in ] ��ǰ�����ȼ�
	vector<CRNRect>	&a_vecRect,				// [in ] ��ռ��Χ
	wstring			a_wstrName,				// [in ] ·��
	vector<double>	a_vecNameLine,			// [in ] ·����״��
	double			a_dPreNameDist,			// [in ] ���·������ʼλ��(����)
	double			a_dNameDispLen,			// [in ] ·������ʾ����
	PST_ROADNAME	a_pRoadName,			// [out] ����·�����ּ���
	double& s_dFontLonSpan, 
	double& s_dFontLatSpan)
{
	// �������ָ���
	int iNameLen = a_wstrName.size();
	// �����С
	double dFontLen = a_dNameDispLen / iNameLen;
	// ������㵽���Ʒֲ��߶����ľ���
	double dDistanceCur = 0.0;

// 	// ȡ��·���ֲ��߶�ʼ������ ���õ�ǰ�������
// 	int iStartIndex = GetPointIndexByDistance(a_stRoadLine.vecPoints, a_dPreNameDist, dDistanceCur);
// 	if (iStartIndex == -1){
// 		// ���Ȳ���
// 		return RET_FAILED;
// 	}

	// ѭ������·��������
	dDistanceCur = a_dPreNameDist + dFontLen / 2;

	static size_t iLineIndex = 0; // ��·��������ʾĳһ��ע�ǵ��Ӧ�ĵ�·
	++iLineIndex;
	for (int i = 0; i < iNameLen; i++)
	{
		a_pRoadName[i].wcText = a_wstrName[i];
		a_pRoadName[i].iLineIndex = iLineIndex;

		// ȡ��ָ������������ϵĵ�
		int iRet = GetPointOnLineByDistance(a_vecNameLine, dDistanceCur, a_pRoadName[i]);
		if (iRet != RET_SUCCESS){
			return iRet;
		}

		dDistanceCur += dFontLen;
	}

// 	// �ж��������򣬵�������˳��
// 	double dMinX = a_pRoadName[0].dX;
// 	double dMinY = a_pRoadName[0].dY;
// 	double dMaxX = a_pRoadName[0].dX;
// 	double dMaxY = a_pRoadName[0].dY;
// 	for (int i = 1; i < iNameLen; i++){
// 		dMinX = min(dMinX, a_pRoadName[i].dX);
// 		dMinY = min(dMinY, a_pRoadName[i].dY);
// 		dMaxX = max(dMaxX, a_pRoadName[i].dX);
// 		dMaxY = max(dMaxY, a_pRoadName[i].dY);
// 	}
	CRNRect clsRect(a_pRoadName[0].dX, a_pRoadName[0].dY);
	clsRect.Inflate(s_dFontLonSpan / 2, s_dFontLatSpan / 2);
	for (int i = 1; i < iNameLen; i++){
		CRNRect clsRectTmp(a_pRoadName[i].dX, a_pRoadName[i].dY);
		clsRectTmp.Inflate(s_dFontLonSpan / 2, s_dFontLatSpan / 2);
		clsRect.Union(clsRectTmp);
	}

	// ·��������
	vector<CRNRect>::iterator iterRect = a_vecRect.begin();
	for (; iterRect != a_vecRect.end(); iterRect++)
	{
		if (iterRect->IsIntersect(clsRect)) {
			return RET_FAILED;
		}
	}
	a_vecRect.push_back(clsRect);

	if (clsRect.Height() > clsRect.Width()){
		// ���θߴ��ڿ���������
		if(a_pRoadName[0].dY < a_pRoadName[iNameLen-1].dY){
			// ��������˳�򣬱�֤���ϵ���
			ReserveName(a_pRoadName, iNameLen);
		}
	}else{
		// ���ο���ڸߣ����ֺ���
		if(a_pRoadName[0].dX > a_pRoadName[iNameLen-1].dX){
			// ��������˳�򣬱�֤������
			ReserveName(a_pRoadName, iNameLen);
		}
	}

	return RET_SUCCESS;
}

// ȡ��ָ��������߶��ϵĵ�
int GetPointOnLineByDistance(vector<double> vecPoints, double a_dDistance, ST_ROADNAME &a_stRoadName)
{
	int		iPointIndex;
	double	dDistance = 0.0;

	iPointIndex = GetPointIndexByDistance(vecPoints, a_dDistance, dDistance);
	if (iPointIndex == -1){
		// �߶γ��Ȳ���
		return RET_FAILED;
	}

	double pstPoint1_x = vecPoints[iPointIndex*2];
	double pstPoint1_y = vecPoints[iPointIndex*2+1];
	double pstPoint2_x = vecPoints[iPointIndex*2+2];
	double pstPoint2_y = vecPoints[iPointIndex*2+3];
	if(fabs(pstPoint2_x - pstPoint1_x) < 1E-15  && fabs(pstPoint2_y - pstPoint1_y) < 1E-15){
		return RET_SUCCESS;
	}

	// ���һ���߶ξ���
	double dDistance2 = CalcSphLen(pstPoint1_x, pstPoint1_y, pstPoint2_x, pstPoint2_y);

	// ����㵽�߶����ľ���
	dDistance = a_dDistance - dDistance;
	// �������ǵȱȹ�ϵ�����㷵������
	a_stRoadName.dX = pstPoint1_x + (pstPoint2_x - pstPoint1_x) * dDistance / dDistance2;
	a_stRoadName.dY = pstPoint1_y + (pstPoint2_y - pstPoint1_y) * dDistance / dDistance2;

	return RET_SUCCESS;
}

// ȡ��ָ��������߶���ʼ�������ţ�-1��ʾ�����ܳ���С��ָ�����룩
int GetPointIndexByDistance(
	vector<double>	a_vecPoints,				// [in ] ����
	double			a_dDistance,				// [in ] ָ�����루�ӵ���ʼ�����㣩
	double			&a_dOutLineDist,			// [out] ��ͷ�ۼƵ����ص�ľ���֮��
	int				a_lStartIndex/* = 0*/,		// [in ] ��ʼ������
	double			a_dStartDist/* = 0*/)		// [in ] ��ʼ����			
{
	int	i;
	double	dTmpDist;	// ĩβ�߶γ���
	if (a_lStartIndex == -1){
		a_lStartIndex = 0;
	}
	a_dOutLineDist = a_dStartDist;
	for (i=a_lStartIndex;i<(a_vecPoints.size()/2);i++){
		dTmpDist = CalcSphLen(a_vecPoints[i*2], a_vecPoints[i*2+1], a_vecPoints[i*2+2], a_vecPoints[i*2+3]);
		if (a_dOutLineDist + dTmpDist > a_dDistance){
			return i;
		}
		a_dOutLineDist += dTmpDist;
	}
	return -1;
}

// ��ת·������˳��
void ReserveName(PST_ROADNAME pstName, int iNameLen)
{
	double dTmp = 0.0;
	for (int i = 0; i < iNameLen / 2; i++)
	{
		// ��������
		dTmp = pstName[i].dX;
		pstName[i].dX = pstName[iNameLen-1-i].dX;
		pstName[iNameLen-1-i].dX = dTmp;

		dTmp = pstName[i].dY;
		pstName[i].dY = pstName[iNameLen-1-i].dY;
		pstName[iNameLen-1-i].dY = dTmp;
	}
}

// ·��ƫ�� ����·����
int CreateRoadNameLine(int iZoom, double dCenterX, PST_ROADLINE pstLine, vector<double> &vecPoints)
{
	bool isLeft = false;
	int iPointCnt = pstLine->vecPoints.size() / 2;

// 	// �ж��������򣬵�������˳��
// 	double dMinX = pstLine->vecPoints[0];
// 	double dMinY = pstLine->vecPoints[1];
// 	double dMaxX = pstLine->vecPoints[0];
// 	double dMaxY = pstLine->vecPoints[1];
// 	for (int i = 1; i < iPointCnt; i++){
// 		dMinX = min(dMinX, pstLine->vecPoints[i*2]);
// 		dMinY = min(dMinY, pstLine->vecPoints[i*2+1]);
// 		dMaxX = max(dMaxX, pstLine->vecPoints[i*2]);
// 		dMaxY = max(dMaxY, pstLine->vecPoints[i*2+1]);
// 	}

	CRNRect clsRect(pstLine->vecPoints[0], pstLine->vecPoints[1]);
	for (int i = 1; i < iPointCnt; i++){
		clsRect.Union(pstLine->vecPoints[i*2], pstLine->vecPoints[i*2+1]);
	}

	if (clsRect.Height() > clsRect.Width()){
		// ���θߴ��ڿ���������
		// ���������ĵ�λ���ж�
		if (pstLine->vecPoints[0] > dCenterX && 
			pstLine->vecPoints[1] > pstLine->vecPoints[(iPointCnt-1)*2+1]) {
			isLeft = true;
		}
		if (pstLine->vecPoints[0] < dCenterX &&
			pstLine->vecPoints[1] < pstLine->vecPoints[(iPointCnt-1)*2+1]) {
			isLeft = true;
		}
	}else{
		// ���ο���ڸߣ����ֺ���
		// ����ƫ��
		if(pstLine->vecPoints[0] > pstLine->vecPoints[(iPointCnt-1)*2]){
			isLeft = true;
		}
	}

	// ��ȡƫ�ƹ�����
	MoveLine(vecPoints, pstLine->vecPoints, isLeft, iZoom);
	MoveLine(vecPoints, vecPoints, isLeft, iZoom);
	MoveLine(vecPoints, vecPoints, isLeft, iZoom);

	return RET_SUCCESS;
}


// ������о���
double GetDistance(vector<double> vecPoints)
{
	double dDistance = 0.0;
	for (int i = 0; i < vecPoints.size()/2-1; i++)
	{
		dDistance += CalcSphLen(vecPoints[i*2], vecPoints[i*2+1], vecPoints[i*2+2], vecPoints[i*2+3]);
	}
	return dDistance;
}

// �������س��� �����׳���
double GetDistanceByPixel(int zoom, double lon, double lat, int len)
{
	LonLat lonlat;
	lonlat.lon = lon;
	lonlat.lat = lat;

	PixelXY pixelXY = convertMercXY2PixelXY(convertLonLat2MercXY(lonlat), zoom);
	PixelXY pixelXY_1 = pixelXY;
	pixelXY_1.X += len;

	LonLat lonlat_1 = convertMercXY2LonLat(convertPixelXY2MercXY(pixelXY_1, zoom));

	return CalcSphLen(lonlat.lon, lonlat.lat, lonlat_1.lon, lonlat_1.lat);
}

// �������س��� ���㾭�ȿ��
double GetLonSpanByPixel(int zoom, double lon, double lat, int len)
{
	LonLat lonlat;
	lonlat.lon = lon;
	lonlat.lat = lat;

	PixelXY pixelXY = convertMercXY2PixelXY(convertLonLat2MercXY(lonlat), zoom);
	PixelXY pixelXY_1 = pixelXY;
	pixelXY_1.X += len;

	LonLat lonlat_1 = convertMercXY2LonLat(convertPixelXY2MercXY(pixelXY_1, zoom));

	return fabs(lonlat_1.lon - lonlat.lon);
}

// �������س��� ����γ�ȿ��
double GetLatSpanByPixel(int zoom, double lon, double lat, int len)
{
	LonLat lonlat;
	lonlat.lon = lon;
	lonlat.lat = lat;

	PixelXY pixelXY = convertMercXY2PixelXY(convertLonLat2MercXY(lonlat), zoom);
	PixelXY pixelXY_1 = pixelXY;
	pixelXY_1.Y += len;

	LonLat lonlat_1 = convertMercXY2LonLat(convertPixelXY2MercXY(pixelXY_1, zoom));

	return fabs(lonlat_1.lat - lonlat.lat);
}
