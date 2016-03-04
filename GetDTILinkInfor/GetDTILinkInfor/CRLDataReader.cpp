#include "CRLDataReader.h"

CRLDataReader::CRLDataReader()
{
}

CRLDataReader::~CRLDataReader()
{
}

void CRLDataReader::GetAllRoadName(map<string, CLinkInfo>& mapAllLink)
{
	vector<string> vtNames;
	string strCurName;
	string strRoadName;

	// 1. ����mesh + linkid, �ҵ���Ӧ��routeid ����
	// 2. ����routeid ���ϣ��ҵ���Ӧ��name����
	// 3. ����name���ϣ��ҵ���һ�����ֵ�·�����û�У���ȡ��һ����·name
	map<string, vector<size_t>>::iterator itr =  m_mapLink2RouteID.begin();
	while( itr !=  m_mapLink2RouteID.end() )
	{
		vtNames.clear();
		for(size_t i = 0; i < itr->second.size(); ++i)
		{
			if( m_RNReader.GetRoadNameByRouteID(itr->second[i], strCurName) )
			{
				vtNames.push_back(strCurName);
			}
		}
		for(size_t i = 0; i < vtNames.size(); ++i)
		{
			if(IncludeChinese(vtNames[i].c_str()) == 1) // ����
			{
				strRoadName = vtNames[i];
				break;
			}
		}
		if(strRoadName.empty() && vtNames.size() > 0)
		{
			strRoadName = vtNames[0];
		}
		if(strRoadName.empty() == false)
		{
			CLinkInfo tempLink;
			tempLink.m_strMeshLinkID = itr->first;
			tempLink.m_strRoadName = strRoadName;
			mapAllLink[itr->first] = tempLink;
		}
		++itr;
	}
}

bool CRLDataReader::GetRoadName(string& strLinkID, string& strRoadName)
{
	bool bRet = false;
	vector<string> vtNames;
	string strCurName;

	// 1. ����mesh + linkid, �ҵ���Ӧ��routeid ����
	// 2. ����routeid ���ϣ��ҵ���Ӧ��name����
	// 3. ����name���ϣ��ҵ���һ�����ֵ�·�����û�У���ȡ��һ����·name
	map<string, vector<size_t>>::iterator itr =  m_mapLink2RouteID.find(strLinkID);
	if( itr !=  m_mapLink2RouteID.end() )
	{
		vtNames.clear();
		strRoadName.clear();
		for(size_t i = 0; i < itr->second.size(); ++i)
		{
			if( m_RNReader.GetRoadNameByRouteID(itr->second[i], strCurName) )
			{
				vtNames.push_back(strCurName);
			}
		}
		for(size_t i = 0; i < vtNames.size(); ++i)
		{
			if(IncludeChinese(vtNames[i].c_str()) == 1) // ����
			{
				bRet = true;
				strRoadName = vtNames[i];
				break;
			}
		}
		if(!bRet && vtNames.size() > 0)
		{
			bRet = true;
			strRoadName = vtNames[0];
		}
	}
	return bRet;
}

bool CRLDataReader::InitialData(string& strRLDataDir, string& strRNDataDir)
{
	bool bRet = false;
	char* pRLDataDir = const_cast<char*>( strRLDataDir.c_str() );
	if (pRLDataDir && MapInfo70::MIInit(CURRENT_API_VERSION))
	{
		long nMapinfoID = MapInfo70::MIOpenTable(pRLDataDir, MODE_RDWR);	
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

	m_RNReader.InitialData( strRNDataDir );
	return bRet;
}
	
bool CRLDataReader::LoadMapData(int nMapinfoID, int nRoadCount)
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

bool CRLDataReader::ParseAttrData( char *pAttrData )
{
	VCHARARRAY vtFiledAarrayValue;
	if(CAttrDataProcess::SplitAttrData(pAttrData,&vtFiledAarrayValue)<=0)
	{		
		return false;
	}	
	string strLinkID(vtFiledAarrayValue[0]);
	strLinkID += ",";
	strLinkID += vtFiledAarrayValue[1];
	
	map<string, vector<size_t>>::iterator itr =  m_mapLink2RouteID.find(strLinkID);
	if( itr ==  m_mapLink2RouteID.end() )
	{
		vector<size_t> temBuf;
		temBuf.push_back( atoi(vtFiledAarrayValue[2].c_str()) );
		m_mapLink2RouteID[strLinkID] =  temBuf;
	}
	else
	{
		itr->second.push_back( atoi(vtFiledAarrayValue[2].c_str()) );
	}
	return true;
}