/**
 * @des: 读取RN表，建立中文routeID到linkID的映射
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
	 * @brief; 初始化,读取tab格式的地图文件
	 */
	bool InitialData( string& strRNDataDir);
	bool GetRoadNameByRouteID(size_t nRouteID, string& strRoadName);
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
	map<size_t, string>		m_mapRouteID2Link;
};

#endif