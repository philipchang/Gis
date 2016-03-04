/***************************************************
 * @File: CRNCreate.h
 * @Description: ��װ��·����λ������ӿڣ��ù��ܸ�������ģ�����
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
// ���Mapinfo�����ֶ���Ŀ
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
	 * @brief; ��ʼ��,��ȡtab��ʽ�ĵ�ͼ�ļ�
	 */
	bool InitialData( char* pDataTabName );

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
	bool ParseAttrData(char *pAttrData,ST_ROADLINE *pMapData);

	/**
	 * @brief: ��ȡһ����·����״��
	 */
	void GetALineShape(MapInfo70::MI_OBJ& Objs,ST_ROADLINE* pMapData);

	// output
	void Output(char* pTabName,vector<ST_ROADNAME>& vecRoadName);
	long CreatMapInfoFile(char* pTabName);		
	void OutputALine( long  MapInfoID, string& strVal, vector<double>& vtPnts);

private:
	vector<string>				m_vtStrFieldName; // mapinfo �ֶ�����
	vector<ST_ROADLINE>			m_vecRoadLine;
};

#endif