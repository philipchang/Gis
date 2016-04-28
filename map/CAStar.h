
/**
 * @des: 
 *		 A star
 *
 */
#ifndef _ASTAT_H
#define _ASTAT_H
#pragma once


#include <vector>
#include <map>
#include "../Common/CommonDef.h"
using namespace std;

typedef vector<const CDTILineData*>	vtRoute;
const	int		MaxRearchNum = 160;

class IinterceForAStar
{
public:
	virtual void ML_GetNearLinks(int nNode, vector<CDTILineData*>& vtNearLinks,bool bIsInput = true){};
};

class CAStar
{
public:
	CAStar();
	~CAStar();
	bool Route_bCalcRoutes(IinterceForAStar* _pSrcLink,const CDTILineData* _pStartLink,const CDTILineData* _pEndLink);
	
private:
	/////////////BFS///////////////
	void Route_vBFSRearch(const CDTILineData* _pStartLink,
							const CDTILineData* _pEndLink);
	void Route_iCalcRoutes(size_t _iRouteIndex);
	bool Route_bCheckRoute(vtRoute* _pRoute, bool _bIsBack = true);
	double Route_dCalcRouteSEDistance(const CDTILineData* _pStartLink,								const	CDTILineData* _pEndLink);

	bool Route_vBacktrack(vtRoute* _pRoute);
	void Route_vGetBestOne();
	/////////////BFS end///////////////

	/////////////DFS///////////////
	void Route_vDFSRearch(const CDTILineData* _pStartLink,
							const CDTILineData* _pEndLink);
	bool Route_bDFS(const CDTILineData* _pCurrLink);
	bool Route_bIsRing(const CDTILineData* _pCurrLink);
	//////////////end/////////////
private:
	
	const CDTILineData*		m_pEndLink;
	IinterceForAStar*		m_pInterceForAStar;
	double					m_pDistanceForSEpoint; // ∆÷’µ„æ‡¿Î

	vtRoute					m_vtRouteTrace; // dfs 
	vector<vtRoute*>		m_vtRearchQuene;
	
	map<const CDTILineData*, int> m_mapOpenLinks; // open 
	map<const CDTILineData*, int> m_mapClosedLinks; // closed
	
};

#endif
