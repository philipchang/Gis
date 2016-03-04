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
	char *pAttri =  strBuf; // ��ȡÿһ�����ݣ������ڱ�buffer
	
	// ��ȡÿһ����¼,�������У�������
	++nRoadCount; // cnt�������е���Ϸ����������Ч���ݷ�Χ[1��cnt+1],
	for(int i = 1; i < nRoadCount; ++i)
	{
		memset(pAttri, 0x00, 1024); // ÿ�����buffer

		// ��ȡ��ǰ��¼��
		if (MapInfo70::MIFetchRow(nMapinfoID, i, pAttri, &Objs) == 0)
		{			
			continue;// ��ȡʧ��
		}
			
		if( !ParseAttrData(pAttri) ) // ������ǰ��¼�ֶ�����
		{
			continue; // ����ʧ��
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
	
	// langue = 1,����
	if( 1 == atoi(vtFiledAarrayValue[13].c_str()) )
	{
		size_t RouteID = atoi(vtFiledAarrayValue[0].c_str());
		m_mapRouteID2Link[ RouteID ] =  vtFiledAarrayValue[2];	
	}
	return true;
}