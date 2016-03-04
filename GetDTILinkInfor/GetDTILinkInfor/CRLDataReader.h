/**
 * @des: ��ȡR���DTIӳ�������meshlink��linkInfo��ӳ��
 *
 */

#ifndef _C_LINK_NAME_H
#define _C_LINK_NAME_H
#pragma once

#include "ComDef.h"
#include "CRNDataReader.h"

class CRLDataReader
{
public:
	CRLDataReader();
	~CRLDataReader();

	/**
	 * @brief; ��ʼ��,��ȡtab��ʽ�ĵ�ͼ�ļ�
	 */
	bool InitialData(string& strRLDataDir, string& strRNDataDir);
	
	bool GetRoadName(string& strLinkID, string& strRoadName);

	void GetAllRoadName(map<string, CLinkInfo>& mapAllLink);
private:

	/**
	 * @brief; ��ȡԭʼ����
	 */
	bool LoadMapData(int nMapinfoID, int nRoadCount);

	/*
	 * @brief: ��ȡ�ֶ�����
	 */
	bool GetMapFileFieldNames(int nMapinfoID);

	/**
	 * @brief; ����һ��Mapinfo ��¼
	 */
	bool ParseAttrData( char *pAttrData );

private:
	
	map<string, vector<size_t>>	m_mapLink2RouteID;		
	CRNDataReader				m_RNReader;
};

#endif