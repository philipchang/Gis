#include "CRNDataReader.h"

CRNDataReader::CRNDataReader()
{
}

CRNDataReader::~CRNDataReader()
{
}
bool CRNDataReader::GetRoadNameByRouteID(size_t nRouteID, string& strRoadName)
{
	bool bRet = false;
	map<size_t, string>::iterator itr = m_mapRouteID2Link.find(nRouteID);
	if( itr != m_mapRouteID2Link.end() )
	{
		strRoadName = itr->second;
		bRet = true;
	}
	return bRet;
}

bool CRNDataReader::InitialData(string& strRNDataDir)
{
	bool bRet = false;
	char* pRNDataDir = const_cast<char*>( strRNDataDir.c_str() );

	if (pRNDataDir && MapInfo70::MIInit(CURRENT_API_VERSION))
	{
		long nMapinfoID = MapInfo70::MIOpenTable(pRNDataDir, MODE_RDWR);	
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
	
bool CRNDataReader::LoadMapData(int nMapinfoID, int nRoadCount)
{
	MapInfo70::MI_OBJ Objs;
	char strBuf[1024]= {0};
	char *pAttri =  strBuf; // 读取每一行数据，缓存在本buffer
	
	// 读取每一条记录,跳过首行（列名）
	++nRoadCount; // cnt是数据行的游戏个数，但有效数据范围[1，cnt+1],
	for(int i = 1; i < nRoadCount; ++i)
	{
		memset(pAttri, 0x00, 1024); // 每次清空buffer

		// 获取当前记录行
		if (MapInfo70::MIFetchRow(nMapinfoID, i, pAttri, &Objs) == 0)
		{			
			continue;// 获取失败
		}
			
		if( !ParseAttrData(pAttri) ) // 解析当前记录字段数据
		{
			continue; // 解析失败
		}
	}
	return true;
}

bool CRNDataReader::ParseAttrData( char *pAttrData )
{
	VCHARARRAY vtFiledAarrayValue;
	if(CAttrDataProcess::SplitAttrData(pAttrData,&vtFiledAarrayValue)<=0)
	{		
		return false;
	}	
	
	// langue = 1,中文
	if( 1 == atoi(vtFiledAarrayValue[13].c_str()) )
	{
		size_t RouteID = atoi(vtFiledAarrayValue[0].c_str());
		m_mapRouteID2Link[ RouteID ] =  vtFiledAarrayValue[2];	
	}
	return true;
}