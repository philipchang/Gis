#include "CRNCreate.h"

CRNCreate::CRNCreate()
{
}

CRNCreate::~CRNCreate()
{
}

void CRNCreate::ProcessRN()
{
	char *pcBasePath = "./srcMap";
	double dCenterX = 0;
	double dCenterY = 0;
	double s_dFontLonSpan = 0.0;
	double s_dFontLatSpan = 0.0;
	char pTabName[256] = {0};

	sprintf_s(pTabName, 256, "%s/lines_test.tab", pcBasePath);	
	InitialData(pTabName);
	if (m_vecRoadLine.size() <= 0) {
		return;
	}

	ReadCentXYFile( "./srcMap/CenterXY.csv",dCenterX, dCenterY);	

	// merge same road
	vector<ST_ROADLINE> vecDesRoadLine;
	CMergeRoader::MergeRoad(m_vecRoadLine,vecDesRoadLine);
	_mkdir("srcMap/RoadName");

	vector<ST_ROADNAME> vecRoadName;
	for (int iZoom = MIN_ZOOM; iZoom <= MAX_ZOOM; iZoom++)
	{
		// proces 10,12,14,16
		if(iZoom % 2 != 0)
			continue;

		// 计算一个汉字的经纬度跨度
		s_dFontLonSpan = GetLonSpanByPixel(iZoom, dCenterX, dCenterY, FONT_WIDTH);
		s_dFontLatSpan = GetLatSpanByPixel(iZoom, dCenterX, dCenterY, FONT_WIDTH);
		
		AutoLabelRoadName(iZoom, dCenterX, vecDesRoadLine, vecRoadName,  s_dFontLonSpan, s_dFontLatSpan);

		sprintf_s(pTabName, 256, "%s/RoadName/RoadName_%d.tab", pcBasePath, iZoom);
		Output(pTabName, vecRoadName);
		vecRoadName.clear();
	}
}

void CRNCreate::ReadCentXYFile(char* _rdsTmcFileDir, double& centX, double& centY )
{
	ifstream  fin( _rdsTmcFileDir  );
	if(fin)
	{
		string strLine;
		getline(fin,strLine); // del title
		getline(fin,strLine);
		
		int a  = strLine.find(',');
		string strY(strLine, a + 1, strLine.size() );
		centX = atof(strLine.c_str());
		centY = atof(strY.c_str());

		fin.close();
	}	
}

bool CRNCreate::InitialData( char* pDataTabName )
{
	bool bRet = false;
	if (pDataTabName && MapInfo70::MIInit(CURRENT_API_VERSION))
	{
		long nMapinfoID = MapInfo70::MIOpenTable(pDataTabName, MODE_RDWR);	
		if (nMapinfoID > 0)		
		{
			long nRoadCount = 0;
			if ((MapInfo70::MIGetNumRows(nMapinfoID, &nRoadCount) != 0)
				&& (nRoadCount > 0))
			{			
				bRet = LoadMapData(nMapinfoID, nRoadCount);					
			}			
			MapInfo70::MICloseTable(nMapinfoID);
		}	
	}
	return bRet;
}
	
bool CRNCreate::LoadMapData(int nMapinfoID, int nRoadCount)
{
	MapInfo70::MI_OBJ Objs;
	char strBuf[1024]= {0};
	char *pAttri =  strBuf; // 读取每一行数据，缓存在本buffer
	
	// 读取字段名称
	GetMapFileFieldNames(nMapinfoID);
	
	// 读取每一条记录,跳过首行（列名）
	++nRoadCount; // cnt是数据行的游戏个数，但有效数据范围[1，cnt+1],
	for(int i = 1; i < nRoadCount; ++i)
	{
		memset(pAttri,0x00,1024); // 每次清空buffer

		// 获取当前记录行
		if (MapInfo70::MIFetchRow(nMapinfoID, i, pAttri, &Objs) == 0)
		{			
			continue;// 获取失败
		}
	
		// 生成道路结构
		ST_ROADLINE stLine;			
		if(!ParseAttrData(pAttri, &stLine)) // 解析当前记录字段数据
		{
			continue; // 解析失败
		}
		
		// 解析当前记录对应的形状数据
		GetALineShape( Objs, &stLine);

		// push到link set中
		m_vecRoadLine.push_back(stLine);
	}

	// 存储单线双向路
	return true;
}

bool CRNCreate::GetMapFileFieldNames(int nMapinfoID)
{
	long nCount = 0; // 获取数据段总个数
	if (MapInfo70::MIGetNumFields(nMapinfoID, &nCount) == 0 ||nCount <= 0 || nCount > MAP_HANDLE_MAPINFO_MAX_SIZE_OF_FIELD)
	{
		return false;
	}

	MapInfo70::MI_ATTRDEF attrdef[MAP_HANDLE_MAPINFO_MAX_SIZE_OF_FIELD];
    MapInfo70::MIGetAttributeDefs(nMapinfoID, attrdef);
	for(int i = 0; i < nCount; ++i)
	{
		char* pField = attrdef[i].name;
		if(pField == NULL)
		{				
			return false;
		}
		m_vtStrFieldName.push_back(string(pField));
	}
	return true;
}

bool CRNCreate::ParseAttrData(char *pAttrData,ST_ROADLINE *pMapData)
{
	VCHARARRAY vtFiledAarrayValue;
	if(CAttrDataProcess::SplitAttrData(pAttrData,&vtFiledAarrayValue)<=0)
	{		
		return false;
	}
	
	int FieldLen = m_vtStrFieldName.size();
	char *pFieldValue = NULL;
	pMapData->flag = 0;
	for(int i = 0; i < FieldLen; ++i)
	{		
		if(m_vtStrFieldName[i].compare("RLCD") == 0)
		{
			pMapData->iRLCD = atoi(vtFiledAarrayValue[i].c_str());
		}
		else if(m_vtStrFieldName[i].compare("DirDes") == 0)
		{
			pMapData->wstrRoadDirDes = DecodeUtf8( vtFiledAarrayValue[i]);
		}
		else if(m_vtStrFieldName[i].compare("RoadName") == 0)
		{
			pMapData->wstrRoadName = DecodeUtf8( vtFiledAarrayValue[i] );
		}
		else if(m_vtStrFieldName[i].compare("Showlev") == 0)
		{
			pMapData->iShowLev = atoi(vtFiledAarrayValue[i].c_str());
		}
		else if(m_vtStrFieldName[i].compare("Drawlev") == 0)
		{
			pMapData->iDrawLev = atoi(vtFiledAarrayValue[i].c_str());
		}		
		else if(m_vtStrFieldName[i].compare("OrderNumber") == 0)
		{
			pMapData->iOrder = atoi(vtFiledAarrayValue[i].c_str());
		}
	}	
	return true;
}

void CRNCreate::GetALineShape(MapInfo70::MI_OBJ& Objs,ST_ROADLINE* pMapData)
{
	switch(Objs.type)
	{
	case MFAL_POINT:			
		break;
	case MFAL_LINE:
		{					
			pMapData->vecPoints.push_back(Objs.obj.line.beg.x);			
			pMapData->vecPoints.push_back( Objs.obj.line.beg.y );
			
			pMapData->vecPoints.push_back(Objs.obj.line.end.x);			
			pMapData->vecPoints.push_back( Objs.obj.line.end.y);
		}
		break;
	case MFAL_PLINE:
		{			
			int nPointCount = Objs.obj.pline.points;
			MapInfo70::DPNT *Pnts = Objs.obj.pline.data;
			for(int iLoopCount = 0; iLoopCount < nPointCount; ++iLoopCount)
			{					
				pMapData->vecPoints.push_back(Pnts[iLoopCount].x);				
				pMapData->vecPoints.push_back(Pnts[iLoopCount].y);
			}				
		}
		break;
	case MFAL_MULTIPLINE:
		{
			int nLineCount = Objs.obj.multipline.plines;
			MapInfo70::MI_POLY *Poly = Objs.obj.multipline.data;
			
			for(int iLoopLine=0; iLoopLine < nLineCount; ++iLoopLine)
			{
				MapInfo70::DPNT * Pnts = Poly[iLoopLine].pPnts;
				for(int j = 0; j < Poly[iLoopLine].points; ++j)
				{					
					pMapData->vecPoints.push_back(Pnts[j].x);
					pMapData->vecPoints.push_back(Pnts[j].y);
				}
			}				
		}
		break;
	case MFAL_REGION:
	case MFAL_RECT:
	case MFAL_ROUNDRECT:
	case MFAL_FONT_POINT:
	case MFAL_CUSTOM_POINT:			
		break;
	}
}

long CRNCreate::CreatMapInfoFile(char* pTabName  )
{
	MapInfo70::MI_COORDSYS CoordSys = { 0 };
	MapInfo70::MI_ATTRDEF miAttr[1] = { 0 };

	MapInfo70::MIInit(CURRENT_API_VERSION);
		
	CoordSys.proj_type = 1;//CSYS_NONEARTH;
	/*CoordSys.parms.parms_union.NonEarth.bIsotropic = TRUE;
	CoordSys.parms.parms_union.NonEarth.SysQuadrant = TRUE;
	CoordSys.parms.parms_union.NonEarth.CoordUnits = UNIT_KM;
	CoordSys.parms.parms_union.NonEarth.UserQuadrant = TRUE;*/
	CoordSys.bounds.x1 = -180;
	CoordSys.bounds.y1 = -180;
	CoordSys.bounds.x2 =  180;
	CoordSys.bounds.y2 =  180;
		
	strcpy(miAttr[0].name , "Name");
	miAttr[0].type = DT_CHAR;
	miAttr[0].width = 64;	
	return MapInfo70::MICreateTable(pTabName, miAttr, 1, &CoordSys, F_NATIVE);
}

void CRNCreate::OutputALine(long  MapInfoID, string& strVal, vector<double>& vtPnts)
{	
	MapInfo70::MI_CPEN   stPen = {2, MapInfo70::PAT_SOLID, MapInfo70::P_SOLID, 0x0f0fcb};//0xA850FF};

	char Buf[256] = {0};		
	sprintf(Buf, "%s",	strVal.c_str());

	long k = 0;	
	int size = vtPnts.size() / 2;
	if(size == 2)
	{
		MapInfo70::MI_OBJ stObj = { 0 };	
		stObj.type = MFAL_LINE;
		stObj.obj.line.beg.x =  vtPnts[0];
		stObj.obj.line.beg.y =  vtPnts[1];
		stObj.obj.line.end.x =  vtPnts[2];
		stObj.obj.line.end.y =  vtPnts[3];
		stObj.obj.line.pen = stPen;
		
		if(!MIInsertRow(MapInfoID, Buf, &stObj, &k))
		{
			printf("Insert MapInfo error!!!!\n");;
		}
	}
	else if (size > 2)
	{
		MapInfo70::MI_OBJ stObj = { 0 };	
		stObj.type = MFAL_PLINE;
		stObj.obj.pline.pen = stPen;
		stObj.obj.pline.points = size; 
		stObj.obj.pline.data =  new  MapInfo70::DPNT [size];

		if(!stObj.obj.pline.data)
		{
			printf("new mapinfo point error!!!!\n");
		}

		for(int i = 0; (i < size);  ++i)
		{
			stObj.obj.pline.data[i].x = vtPnts[2*i];
			stObj.obj.pline.data[i].y = vtPnts[2*i + 1];
		}
	
		if(!MIInsertRow(MapInfoID, Buf, &stObj, &k))
		{
			printf("Insert MapInfo error!!!!\n");
		}
		delete[] stObj.obj.pline.data;
	}
}

void CRNCreate::Output(char* pTabName,vector<ST_ROADNAME>& vecRoadName)
{
	long mapFileID = CreatMapInfoFile(pTabName);
	if(mapFileID > 0)
	{
		//输出
		int mergyStart = 0;
		int mergyEnd = 0;
		int Len = vecRoadName.size();
		vector<double> vtPnts;

		for(int i = 1; i < Len; ++i)
		{	
			if(i != (Len - 1))
			{
				if(	vecRoadName[i].iLineIndex == vecRoadName[mergyStart].iLineIndex)
				{
					mergyEnd = i;
					continue;
				}
			}
			else
			{
				mergyEnd = i; // the laset one
			}
		
			wstring wstr;			
			for(int j = mergyStart;  j <= mergyEnd; ++j)
			{
				vtPnts.push_back( vecRoadName[j].dX );
				vtPnts.push_back( vecRoadName[j].dY );
				wstr += vecRoadName[j].wcText;
			}			
			string strVal = EncodeUtf8(wstr);
			OutputALine( mapFileID, strVal, vtPnts);

			vtPnts.clear();
			mergyEnd = i;
			mergyStart = i; // reset
		}
		MapInfo70::MICloseTable(mapFileID);
	}
}