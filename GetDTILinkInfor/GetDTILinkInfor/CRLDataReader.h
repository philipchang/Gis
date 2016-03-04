/**
 * @des: 读取R表和DTI映射表，建立meshlink到linkInfo的映射
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
	 * @brief; 初始化,读取tab格式的地图文件
	 */
	bool InitialData(string& strRLDataDir, string& strRNDataDir);
	
	bool GetRoadName(string& strLinkID, string& strRoadName);

	void GetAllRoadName(map<string, CLinkInfo>& mapAllLink);
private:

	/**
	 * @brief; 读取原始数据
	 */
	bool LoadMapData(int nMapinfoID, int nRoadCount);

	/*
	 * @brief: 读取字段名称
	 */
	bool GetMapFileFieldNames(int nMapinfoID);

	/**
	 * @brief; 解析一条Mapinfo 记录
	 */
	bool ParseAttrData( char *pAttrData );

private:
	
	map<string, vector<size_t>>	m_mapLink2RouteID;		
	CRNDataReader				m_RNReader;
};

#endif