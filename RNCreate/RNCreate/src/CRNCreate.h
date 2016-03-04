/***************************************************
 * @File: CRNCreate.h
 * @Description: 封装道路名称位置排序接口，该功能复用其他模块代码
 * @Author: changggy 2015-4-23
 * copy all right by AutoPia
 ***************************************************/

#ifndef _CRNCREAT_H
#define _CRNCREAT_H
#pragma once

#include "RNCreate.h"
#include "CMergeRoader.h"
#include "Common.h"
#include "AttrDataProcess.h"
#include "mapInfo/MFAL.H"

#include <direct.h>
// 最大Mapinfo数据字段数目
#define  MAP_HANDLE_MAPINFO_MAX_SIZE_OF_FIELD 256

class CRNCreate
{
public:
	CRNCreate();
	~CRNCreate();

	void ProcessRN();

private:
	void ReadCentXYFile(char* _rdsTmcFileDir, double& centX, double& centY );


	/**
	 * @brief; 初始化,读取tab格式的地图文件
	 */
	bool InitialData( char* pDataTabName );

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
	bool ParseAttrData(char *pAttrData,ST_ROADLINE *pMapData);

	/**
	 * @brief: 获取一条道路的形状点
	 */
	void GetALineShape(MapInfo70::MI_OBJ& Objs,ST_ROADLINE* pMapData);

	// output
	void Output(char* pTabName,vector<ST_ROADNAME>& vecRoadName);
	long CreatMapInfoFile(char* pTabName);		
	void OutputALine( long  MapInfoID, string& strVal, vector<double>& vtPnts);

private:
	vector<string>				m_vtStrFieldName; // mapinfo 字段名称
	vector<ST_ROADLINE>			m_vecRoadLine;
};

#endif