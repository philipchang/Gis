#include "CRNCreate.h"


int main(int argc,char **argv)
{
	CRNCreate rnCreator;
	rnCreator.ProcessRN();
		
	printf("道路名称编译完成\n");
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

		// 计算一个汉字的经纬度跨度
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
	
	printf("道路名称编译完成\n");
	system("pause");
	return RET_SUCCESS;
}

*/
/************************************************************************/
/* 道路名称排布                                                         */
/************************************************************************/

// 路名自动放置
int AutoLabelRoadName(int a_iZoomLevel, double a_dCenterX, vector<ST_ROADLINE> &a_vecRoadLine,
	vector<ST_ROADNAME> &a_vecRoadName,double& s_dFontLonSpan, 	double& s_dFontLatSpan)
{
	if (a_vecRoadLine.size() <= 0) {
		return RET_SUCCESS;
	}

	double	dTotalDist = 0.0;			// 路线长度
	double	dPreNameDist = 0.0;			// 路名起始长度
	double	dNameDispLen = 0.0;			// 路名显示长度

	vector<CRNRect> vecRect;			// 路名排重范围

	// 遍历全部道路
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

		// 查找起终点相同道路
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
				// gaojian test iDrawLev 定义 存在疑虑
				else if (pstLineTmp->iDrawLev < pstLineTmp->iDrawLev)
				{
					pstLine = pstLineTmp;
				}
			}
		}


		// 开始生成路名

		// 路线偏移 生成路名线
		vector<double> vecNameLine;
		CreateRoadNameLine(a_iZoomLevel, a_dCenterX, pstLine, vecNameLine);

		// 路名文字数量
		int iNameLen = pstLine->wstrRoadName.size();
		if (iNameLen <= 0) {
			continue;
		}

		// 计算线段距离(MKT 米单位)
		dTotalDist = GetDistance(vecNameLine);
		if (dTotalDist <= 0) {
			continue;
		}

		// 计算文字长度
		int iNameDispLenPx = FONT_WIDTH * iNameLen;
		dNameDispLen = GetDistanceByPixel(a_iZoomLevel, vecNameLine[0], vecNameLine[1], iNameDispLenPx);

		dPreNameDist = (dTotalDist - dNameDispLen) / 2;
		if (dPreNameDist < 0 || dPreNameDist >= dTotalDist) {
			continue;
		}

		// 生成道路名称
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
				//printf("比例尺[%d] 创建路名 出错\n", a_iZoomLevel);
			}
			delete pstRoadName;
		}
	}

	// 恢复标记
	iterLine = a_vecRoadLine.begin();
	for (; iterLine != a_vecRoadLine.end(); iterLine++)
	{
		iterLine->flag = 0;
	}


// 	// 遍历全部道路
// 	vector<ST_ROADLINE>::iterator iterLine = a_vecRoadLine.begin();
// 	for (; iterLine != a_vecRoadLine.end(); iterLine++)
// 	{
// 		ST_ROADLINE &stLine = *iterLine;
// 
// 		// 路名文字数量
// 		int iNameLen = stLine.wstrRoadName.size();
// 		if (iNameLen <= 0) {
// 			continue;
// 		}
// 
// 		// 计算线段距离(MKT 米单位)
// 		dTotalDist = GetDistance(stLine.vecPoints);
// 		if (dTotalDist <= 0) {
// 			continue;
// 		}
// 
// 		// 计算文字长度
// 		int iNameDispLenPx = FONT_WIDTH * iNameLen;
// 		dNameDispLen = GetDistanceByPixel(a_iZoomLevel, stLine.vecPoints[0], stLine.vecPoints[1], iNameDispLenPx);
// 
// 		dPreNameDist = (dTotalDist - dNameDispLen) / 2;
// 		if (dPreNameDist < 0 || dPreNameDist >= dTotalDist) {
// 			return RET_SUCCESS;
// 		}
// 
// 		// 生成道路名称
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
// 				printf("比例尺[%d] 创建路名 出错\n", a_iZoomLevel);
// 			}
// 		}
// 	}

	return RET_SUCCESS;
}

// 根据线段分布道路名称
int CreateRoadName(
	int				a_iZoomLevel,			// [in ] 当前比例等级
	vector<CRNRect>	&a_vecRect,				// [in ] 已占范围
	wstring			a_wstrName,				// [in ] 路名
	vector<double>	a_vecNameLine,			// [in ] 路名形状线
	double			a_dPreNameDist,			// [in ] 输出路名的起始位置(距离)
	double			a_dNameDispLen,			// [in ] 路名的显示长度
	PST_ROADNAME	a_pRoadName,			// [out] 单条路线文字集合
	double& s_dFontLonSpan, 
	double& s_dFontLatSpan)
{
	// 名称文字个数
	int iNameLen = a_wstrName.size();
	// 字体大小
	double dFontLen = a_dNameDispLen / iNameLen;
	// 折线起点到名称分布线段起点的距离
	double dDistanceCur = 0.0;

// 	// 取得路名分布线段始点索引 借用当前距离变量
// 	int iStartIndex = GetPointIndexByDistance(a_stRoadLine.vecPoints, a_dPreNameDist, dDistanceCur);
// 	if (iStartIndex == -1){
// 		// 长度不足
// 		return RET_FAILED;
// 	}

	// 循环处理路名各文字
	dDistanceCur = a_dPreNameDist + dFontLen / 2;

	static size_t iLineIndex = 0; // 道路索引，标示某一个注记点对应的道路
	++iLineIndex;
	for (int i = 0; i < iNameLen; i++)
	{
		a_pRoadName[i].wcText = a_wstrName[i];
		a_pRoadName[i].iLineIndex = iLineIndex;

		// 取得指定距离的折线上的点
		int iRet = GetPointOnLineByDistance(a_vecNameLine, dDistanceCur, a_pRoadName[i]);
		if (iRet != RET_SUCCESS){
			return iRet;
		}

		dDistanceCur += dFontLen;
	}

// 	// 判断文字走向，调换点列顺序
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

	// 路名简单排重
	vector<CRNRect>::iterator iterRect = a_vecRect.begin();
	for (; iterRect != a_vecRect.end(); iterRect++)
	{
		if (iterRect->IsIntersect(clsRect)) {
			return RET_FAILED;
		}
	}
	a_vecRect.push_back(clsRect);

	if (clsRect.Height() > clsRect.Width()){
		// 矩形高大于宽，文字竖排
		if(a_pRoadName[0].dY < a_pRoadName[iNameLen-1].dY){
			// 调换点列顺序，保证从上到下
			ReserveName(a_pRoadName, iNameLen);
		}
	}else{
		// 矩形宽大于高，文字横排
		if(a_pRoadName[0].dX > a_pRoadName[iNameLen-1].dX){
			// 调换点列顺序，保证从左到右
			ReserveName(a_pRoadName, iNameLen);
		}
	}

	return RET_SUCCESS;
}

// 取得指定距离的线段上的点
int GetPointOnLineByDistance(vector<double> vecPoints, double a_dDistance, ST_ROADNAME &a_stRoadName)
{
	int		iPointIndex;
	double	dDistance = 0.0;

	iPointIndex = GetPointIndexByDistance(vecPoints, a_dDistance, dDistance);
	if (iPointIndex == -1){
		// 线段长度不足
		return RET_FAILED;
	}

	double pstPoint1_x = vecPoints[iPointIndex*2];
	double pstPoint1_y = vecPoints[iPointIndex*2+1];
	double pstPoint2_x = vecPoints[iPointIndex*2+2];
	double pstPoint2_y = vecPoints[iPointIndex*2+3];
	if(fabs(pstPoint2_x - pstPoint1_x) < 1E-15  && fabs(pstPoint2_y - pstPoint1_y) < 1E-15){
		return RET_SUCCESS;
	}

	// 最后一段线段距离
	double dDistance2 = CalcSphLen(pstPoint1_x, pstPoint1_y, pstPoint2_x, pstPoint2_y);

	// 插入点到线段起点的距离
	dDistance = a_dDistance - dDistance;
	// 根据三角等比关系，计算返回坐标
	a_stRoadName.dX = pstPoint1_x + (pstPoint2_x - pstPoint1_x) * dDistance / dDistance2;
	a_stRoadName.dY = pstPoint1_y + (pstPoint2_y - pstPoint1_y) * dDistance / dDistance2;

	return RET_SUCCESS;
}

// 取得指定距离的线段起始点索引号（-1表示折线总长度小于指定距离）
int GetPointIndexByDistance(
	vector<double>	a_vecPoints,				// [in ] 点列
	double			a_dDistance,				// [in ] 指定距离（从点列始点起算）
	double			&a_dOutLineDist,			// [out] 从头累计到返回点的距离之和
	int				a_lStartIndex/* = 0*/,		// [in ] 起始点索引
	double			a_dStartDist/* = 0*/)		// [in ] 起始距离			
{
	int	i;
	double	dTmpDist;	// 末尾线段长度
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

// 倒转路名文字顺序
void ReserveName(PST_ROADNAME pstName, int iNameLen)
{
	double dTmp = 0.0;
	for (int i = 0; i < iNameLen / 2; i++)
	{
		// 交换坐标
		dTmp = pstName[i].dX;
		pstName[i].dX = pstName[iNameLen-1-i].dX;
		pstName[iNameLen-1-i].dX = dTmp;

		dTmp = pstName[i].dY;
		pstName[i].dY = pstName[iNameLen-1-i].dY;
		pstName[iNameLen-1-i].dY = dTmp;
	}
}

// 路线偏移 生成路名线
int CreateRoadNameLine(int iZoom, double dCenterX, PST_ROADLINE pstLine, vector<double> &vecPoints)
{
	bool isLeft = false;
	int iPointCnt = pstLine->vecPoints.size() / 2;

// 	// 判断文字走向，调换点列顺序
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
		// 矩形高大于宽，文字竖排
		// 根据离中心点位置判断
		if (pstLine->vecPoints[0] > dCenterX && 
			pstLine->vecPoints[1] > pstLine->vecPoints[(iPointCnt-1)*2+1]) {
			isLeft = true;
		}
		if (pstLine->vecPoints[0] < dCenterX &&
			pstLine->vecPoints[1] < pstLine->vecPoints[(iPointCnt-1)*2+1]) {
			isLeft = true;
		}
	}else{
		// 矩形宽大于高，文字横排
		// 向下偏移
		if(pstLine->vecPoints[0] > pstLine->vecPoints[(iPointCnt-1)*2]){
			isLeft = true;
		}
	}

	// 获取偏移过的线
	MoveLine(vecPoints, pstLine->vecPoints, isLeft, iZoom);
	MoveLine(vecPoints, vecPoints, isLeft, iZoom);
	MoveLine(vecPoints, vecPoints, isLeft, iZoom);

	return RET_SUCCESS;
}


// 计算点列距离
double GetDistance(vector<double> vecPoints)
{
	double dDistance = 0.0;
	for (int i = 0; i < vecPoints.size()/2-1; i++)
	{
		dDistance += CalcSphLen(vecPoints[i*2], vecPoints[i*2+1], vecPoints[i*2+2], vecPoints[i*2+3]);
	}
	return dDistance;
}

// 根据像素长度 计算米长度
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

// 根据像素长度 计算经度跨度
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

// 根据像素长度 计算纬度跨度
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
