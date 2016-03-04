/**
 * @des: 读取R表和DTI映射表，建立meshlink到linkInfo的映射
 *
 */

#ifndef _CGET_DTI_LINK_H
#define _CGET_DTI_LINK_H
#pragma once

#include "ComDef.h"
#include "CRLDataReader.h"

class CGetDTILink
{
public:
	CGetDTILink();
	~CGetDTILink();

	/**
	 * @brief; 初始化,读取tab格式的地图文件
	 * @input  nOperator,操作类型，
	 *			1是输出dti对应的道路名称
	 *          2是输出所有道路名称
	 */
	bool Process(string& strCityDir, int nOperator );		
private:
	void ReadDti();
	void PushOneDtiInfo(string& strDtiInfo);
	void PushRoadName();
	void Output(string& strOutputCity);
	void LoopFiles(string strCityDir);
	
private:
	size_t					m_nADCode;
	string					m_strR_LNameDir;
	string					m_strR_NameDir;
	string					m_strDITCsvDir;

	map<string, CLinkInfo>	m_mapLinkID2Info;//<meshlink, stLinkInfor>

	CRLDataReader			m_RLReader;
};


#endif