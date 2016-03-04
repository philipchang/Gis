#include "CGetDTILink.h"

CGetDTILink::CGetDTILink():m_nADCode(0)
{
}

CGetDTILink::~CGetDTILink()
{
	//map<string, CLinkInfo>().swap(m_mapLinkID2Info);
}

bool CGetDTILink::Process(string& strCityDir, int nOperator)
{
	// 1. 查找各个文件，dti匹配表，RName,RLname表
	LoopFiles( strCityDir);
		
	// 2. 读取RName,RLname 表
	m_RLReader.InitialData(m_strR_LNameDir, m_strR_NameDir);

	// 3. 根据不同的类型，提取不同的link名称
	if(1 == nOperator)//只提取dti道路
	{
		ReadDti();
		PushRoadName();
	} else {// 提取所有道路
		m_mapLinkID2Info.clear();
		m_RLReader.GetAllRoadName(m_mapLinkID2Info);
	}

	Output(strCityDir);
	return true;
}

void CGetDTILink::ReadDti()
{
	ifstream  fin( m_strDITCsvDir.c_str() );
	if(fin)
	{
		string strLine;
		getline(fin,strLine); // del title

		if( 0 == strLine.compare("CITYID,MESHID,CLASS,LOCID,MAPID,LINKID,DIR,DIST,SEQNUM"))
		{
			while(getline(fin,strLine))
			{			
				PushOneDtiInfo( strLine );
			}
		} else {
			cout<<"\n ******************************\n ReadDti file format error\n ******************************\n";
		}
	}
	fin.close();
}

void CGetDTILink::PushOneDtiInfo(string& strDtiInfo)
{
	if( strDtiInfo.empty() )
		return;

	vector<string> vtstrs;
	string strBreak(",");
	DivString(strDtiInfo, strBreak, vtstrs);	

	string strLink(vtstrs[4]);
	strLink += ",";
	strLink += vtstrs[5];

	string strDTILcs; // mesh,cls,dtiloc
	strDTILcs += vtstrs[1];
	strDTILcs += strBreak;
	strDTILcs += vtstrs[2];
	strDTILcs += strBreak;
	strDTILcs += vtstrs[3];	
	
	string strMeshLinkID; // mesh,linkid
	strMeshLinkID += vtstrs[4];
	strMeshLinkID += strBreak;
	strMeshLinkID += vtstrs[5];
	
	CLinkInfo	linkInfo;
	linkInfo.m_strLocIndex = strDTILcs;
	linkInfo.m_nCls = atoi( vtstrs[2].c_str() );
	linkInfo.m_dLength = atof( vtstrs[7].c_str() );
	linkInfo.m_strMeshLinkID = strMeshLinkID;

	m_mapLinkID2Info[strLink] = linkInfo;
	m_nADCode = atoi( vtstrs[0].c_str() );
}

void CGetDTILink::PushRoadName()
{
	string strLinkID;
	map<string, CLinkInfo>::iterator itr = m_mapLinkID2Info.begin();
	for(; itr != m_mapLinkID2Info.end(); ++itr)
	{
		strLinkID = itr->first;
		if(false == m_RLReader.GetRoadName(strLinkID, itr->second.m_strRoadName))
			cout<<strLinkID.c_str()<<"  can not find name\n";
	}
}

void CGetDTILink::Output(string& strOutputCity)
{
	char strOutFile[128] = {0};
	sprintf(strOutFile,"%s/DTILink_Name.csv",strOutputCity.c_str());

	ofstream  fout( strOutFile );
	if(fout)
	{
		// set title
		fout<<"RoadName,Cls,Funcls,Length,Loc_Mesh,Loc_Cls,Loc_ID,Mesh,LinkID\n";
		
		string strBreak(",");
		map<string, CLinkInfo>::iterator itr = m_mapLinkID2Info.begin();
		for(; itr != m_mapLinkID2Info.end(); ++itr)
		{
			if(itr->second.m_strRoadName.size() > 0)
			{
				char strTmp[256] = {0};
				sprintf(strTmp, "%s,%d,0c01,%f,%s,%s\n",
					itr->second.m_strRoadName.c_str(),
						itr->second.m_nCls,
						itr->second.m_dLength,
						itr->second.m_strLocIndex.c_str(),
						itr->second.m_strMeshLinkID.c_str());
				fout<<strTmp;
			}
		}
		fout.close();
	}
}

void CGetDTILink::LoopFiles(string strCityDir)
{	
	char DirFind[256] = {0};
	sprintf(DirFind,"%s/*.*", strCityDir.c_str());
	struct _finddata_t FileInfo;
	long FileHandle = _findfirst(DirFind, &FileInfo);
	
	strCityDir += "/";
	if(FileHandle != -1)
	{
		do{
			int fileNameLen = strlen(FileInfo.name);
			if( fileNameLen > 4 )
			{
				if(strcmp(".csv", &FileInfo.name[fileNameLen - 4]) == 0)
				{
					m_strDITCsvDir += strCityDir;
					m_strDITCsvDir += FileInfo.name;
				}
				if(strcmp(".TAB", &FileInfo.name[fileNameLen - 4]) == 0)
				{
					if(strncmp("R_LName", FileInfo.name, 6) == 0)
					{
						m_strR_LNameDir += strCityDir;
						m_strR_LNameDir += FileInfo.name;
					} else if(strncmp("R_Name", FileInfo.name, 5) == 0)
					{
						m_strR_NameDir += strCityDir;
						m_strR_NameDir += FileInfo.name;
					}
				}				
			}

		}while(_findnext(FileHandle, &FileInfo) == 0);
		_findclose(FileHandle);
	}
}