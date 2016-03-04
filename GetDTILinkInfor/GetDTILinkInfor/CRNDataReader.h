/**
 * @des: ��ȡRN����������routeID��linkID��ӳ��
 *
 */

#ifndef _C_ROUTE_NAME_H
#define _C_ROUTE_NAME_H
#pragma once

#include "ComDef.h"

class CRNDataReader
{
public:
	CRNDataReader();
	~CRNDataReader();

	/**
	 * @brief; ��ʼ��,��ȡtab��ʽ�ĵ�ͼ�ļ�
	 */
	bool InitialData( string& strRNDataDir);
	bool GetRoadNameByRouteID(size_t nRouteID, string& strRoadName);
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
	map<size_t, string>		m_mapRouteID2Link;
};

#endif