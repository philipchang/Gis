/***************************************************
 * @File: CMergeRoader.h
 * @Description: merge road with roadname+dirdes to a long road
 * @Author: changggy 2014-12-5
 * copy all right by AutoPia
 ***************************************************/

#pragma once

#include "DataAcc.h"
#include <map>
#include <string>
#include <algorithm>
using namespace std;
const int VALID_SHOW_LEV = 16;

class CMergeBJ_RingRoad
{
public:
	CMergeBJ_RingRoad()
	{

		m_strBJRings["北三环"] = false;
		m_strBJRings["北四环"] = false;
		m_strBJRings["北五环"] = false;

		m_strBJRings["南三环"] = false;
		m_strBJRings["南四环"] = false;
		m_strBJRings["南五环"] = false;

		m_strBJRings["东三环"] = false;
		m_strBJRings["东四环"] = false;
		m_strBJRings["东五环"] = false;

		m_strBJRings["西三环"] = false;
		m_strBJRings["西四环"] = false;
		m_strBJRings["西五环"] = false;
	}

	bool IsValidRing(wstring strRoadName)
	{
		bool bRet = true;
		map<string, bool>::iterator itr = m_strBJRings.find(EncodeUtf8(strRoadName));
		if(m_strBJRings.end() != itr)
		{
			if(itr->second == true)
				bRet = false;

			itr->second = true;
		}
		return bRet;
	}

	map<string, bool> m_strBJRings;
};

class CMergeRoader
{
public:
	
	static void MergeRoad(vector<ST_ROADLINE>& vecRoadLine, vector<ST_ROADLINE>& vecDesRoadLine)
	{
		static CMergeBJ_RingRoad bjRings;

		// base road name + dir des, store the same roads
		map<wstring, vector<PST_ROADLINE>> mapRN2Roads;
		for(size_t i = 0; i < vecRoadLine.size(); ++i)
		{
			if(vecRoadLine[i].iShowLev > VALID_SHOW_LEV)
				continue;

			// key: roadname+dirdes+showlev+drawlev+iRLCD
			wstring strKey = vecRoadLine[i].wstrRoadName;
			strKey += vecRoadLine[i].wstrRoadDirDes;
			strKey += vecRoadLine[i].iShowLev;
			strKey += vecRoadLine[i].iDrawLev;
			strKey += vecRoadLine[i].iRLCD;

			map<wstring, vector<PST_ROADLINE>>::iterator itrFind = mapRN2Roads.find(strKey);
			if( itrFind == mapRN2Roads.end() )
			{
				if(bjRings.IsValidRing(vecRoadLine[i].wstrRoadName) )
				{
					vector<PST_ROADLINE> vtTempBuf;
					vtTempBuf.push_back(&vecRoadLine[i]);
					mapRN2Roads[strKey] = vtTempBuf;
				}
			}
			else
			{
				itrFind->second.push_back(&vecRoadLine[i]);
			}
		}

		// base above map, merge same road to a road
		map<wstring, vector<PST_ROADLINE>>::iterator itr = mapRN2Roads.begin();
		for(; itr != mapRN2Roads.end(); ++itr)
		{
			// order same roads
			sort(itr->second.begin(), itr->second.end(), RoadLineCmp);

			ST_ROADLINE stTempRoad =  *(itr->second[0]);

			// merge road shape point
			size_t roadLen = itr->second.size();
			if( roadLen > 1)
			{
				for(size_t i = 1; i < roadLen; ++i)
				{
					size_t curRoadPntLen = itr->second[i]->vecPoints.size();
					for(size_t j = 0; j < curRoadPntLen; ++j)
						stTempRoad.vecPoints.push_back(itr->second[i]->vecPoints[j]);
				}
			}
			vecDesRoadLine.push_back(stTempRoad);
		}
	}
};