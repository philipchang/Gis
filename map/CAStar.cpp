#include "CAStar.h"
#include <algorithm>

/////////////Test//////////////////////////
long gFileID =  0,nSuccessNum = 0;
long CreateResultTable()
{
	MapInfo70::MI_COORDSYS CoordSys = { 0 };
	MapInfo70::MI_ATTRDEF miAttr[1] = { 0 };
	MapInfo70::MIInit(CURRENT_API_VERSION);
		
	CoordSys.proj_type = 1;
	CoordSys.bounds.x1 = -180;
	CoordSys.bounds.y1 = -180;
	CoordSys.bounds.x2 =  180;
	CoordSys.bounds.y2 =  180;

	strcpy_s(miAttr[0].name , "id");
	miAttr[0].type = DT_INTEGER;
	
	return MapInfo70::MICreateTable("tmclink.tab", miAttr, 1, &CoordSys, F_NATIVE);
}

void OutputPointTMCSegInfo(const vector<double>& vtShape, int nID)
{
	MapInfo70::MI_CPEN   stPen = {2, MapInfo70::PAT_SOLID, MapInfo70::P_SOLID, 0x0f0fcb};//0xA850FF};
	char Buf[256] = {0};
	long k = 0;		
	sprintf_s(Buf, "%d",nID);

	int size = vtShape.size() / 2;
	if(size == 2)
	{
		MapInfo70::MI_OBJ stObj = { 0 };	
		stObj.type = MFAL_LINE;
		stObj.obj.line.beg.x =  vtShape[0];
		stObj.obj.line.beg.y =  vtShape[1];
		stObj.obj.line.end.x =  vtShape[2];
		stObj.obj.line.end.y =  vtShape[3];
		stObj.obj.line.pen = stPen;
		
		if(!MIInsertRow(gFileID, Buf, &stObj, &k))
		{
			printf("Insert MapInfo error!!!!\n");;
		}
	}
	else if (size > 2)
	{
		MapInfo70::MI_OBJ stObj = { 0 };	
		stObj.type = MFAL_PLINE;
		stObj.obj.pline.pen = stPen;
		stObj.obj.pline.points = size; 
		stObj.obj.pline.data =  new  MapInfo70::DPNT [size];

		if(!stObj.obj.pline.data)
		{
			printf("new mapinfo point error!!!!\n");
		}

		for(int i = 0; (i < size);  ++i)
		{
			stObj.obj.pline.data[i].x = vtShape[2*i];
			stObj.obj.pline.data[i].y = vtShape[2*i + 1];
		}
	
		if(!MIInsertRow(gFileID, Buf, &stObj, &k))
		{
			printf("Insert MapInfo error!!!!\n");
		}
		delete[] stObj.obj.pline.data;
	}
}

void GetShape(vtRoute* pRoute, vector<double>& vtShape)
{
	size_t nItemNum = pRoute->size();
	const CDTILineData* pCurrentLink = NULL;
	for(size_t i = 0; i< nItemNum; ++i)
	{
		// 追加道路路线
		pCurrentLink = (*pRoute)[i];
		if(pCurrentLink->m_sDir == 3) // 路形状反方向
		{
			// 路形是xyxy,倒序取
			int nLen = pCurrentLink->m_vtPnts.size();
			for(int j = nLen - 2; j > -1; )
			{
				vtShape.push_back(pCurrentLink->m_vtPnts[j]);
				vtShape.push_back(pCurrentLink->m_vtPnts[j + 1]);
				j -= 2;
			}

		}else{
			vtShape.insert(vtShape.end(), pCurrentLink->m_vtPnts.begin(), 	pCurrentLink->m_vtPnts.end() );
		}
	}
}
void OutputOneRoute(vtRoute* pRoute, int nID)
{
	if(gFileID < 1)
		return;
	vector<double> vtShape;
	GetShape(pRoute, vtShape);
	OutputPointTMCSegInfo(vtShape, nID);
}
/////////////end//////////////////////////////

CAStar::CAStar():m_pEndLink(NULL),m_pInterceForAStar(NULL)
{
	for(size_t i = 0; i < MaxRearchNum; ++i)
		m_vtRearchQuene.push_back(new vtRoute);
}

CAStar::~CAStar()
{
	for(size_t i = 0; i < m_vtRearchQuene.size(); ++i)
		delete m_vtRearchQuene[i];
}

bool CAStar::Route_bCalcRoutes(IinterceForAStar* _pSrcLink,const CDTILineData* _pStartLink,const CDTILineData* _pEndLink)
{
	m_pEndLink = _pEndLink;
	m_pInterceForAStar = _pSrcLink; // for cb
	m_pDistanceForSEpoint = Route_dCalcRouteSEDistance(_pStartLink,_pEndLink);

	// todo test
	gFileID =  CreateResultTable();
	
	//Route_vDFSRearch(_pStartLink, _pEndLink);
	Route_vBFSRearch(_pStartLink, _pEndLink);

	MapInfo70::MICloseTable(gFileID);
	return (nSuccessNum > 0);
}

/////////////BFS//////////////////////////////
void CAStar::Route_vBFSRearch(const CDTILineData* _pStartLink,												 const CDTILineData* _pEndLink)
{
	bool bNeedFind = false;

	// 1. set root
	m_vtRearchQuene[0]->push_back(_pStartLink);
	m_mapOpenLinks[_pStartLink] = 1;

	// 2. loop links for every route
	while(1)
	{
		bNeedFind = false;
		nSuccessNum = 0;

		// 2.1 count the success route num, and loop link for no ok routes
		for(size_t i = 0; i < m_vtRearchQuene.size(); ++i)
		{
			if((m_vtRearchQuene[i])&&(m_vtRearchQuene[i]->size() > 0))
			{
				if(m_vtRearchQuene[i]->back() == _pEndLink)
				{
					++nSuccessNum;
				}else{
					bNeedFind = true;
					Route_iCalcRoutes(i); // calc current route
				}
			}
		}

		if(!bNeedFind) // 2.2 all route are ok
			break;

		// 2.3 get min routes
		if(nSuccessNum > (MaxRearchNum - 3)) 
			break;

		// 2.4 can not go to the end link
		//if() 
			//break;
	}

	// 3. get best route
	Route_vGetBestOne();
}
void CAStar::Route_vGetBestOne()
{
	double dMaxDist = 0xfffffffffff;
	size_t iMaxIndex = 0;
	for(size_t i = 0; i < m_vtRearchQuene.size(); ++i)
	{
		if((m_vtRearchQuene[i])&&(m_vtRearchQuene[i]->size() > 0))
		{
			if(m_vtRearchQuene[i]->back() == m_pEndLink)
			{
				OutputOneRoute(m_vtRearchQuene[i],3+i);
				{
					vtRoute* _pRoute = m_vtRearchQuene[i];

					// 1. base distance
					double dTotalRouteDis = 0.0f;
					for(size_t j = 0; j < _pRoute->size(); ++j)
						dTotalRouteDis += (*_pRoute)[j]->m_sRoadLen;
					if(dMaxDist > dTotalRouteDis){
						iMaxIndex = i+3;
						dMaxDist = dTotalRouteDis;
					}
				}
			}
		}
	}
	return;
}

void CAStar::Route_iCalcRoutes(size_t _iRouteIndex)
{
	vtRoute* pRoute = m_vtRearchQuene[_iRouteIndex];
	if(!pRoute)
		return;

	int nNode = pRoute->back()->m_nEndNode;
	vector<CDTILineData*> vtNearLinks;

	// 1. Get next links
	m_pInterceForAStar->ML_GetNearLinks(nNode, vtNearLinks, true);
	
	// filte the closed link
	vector<CDTILineData*>::iterator itr = vtNearLinks.begin();
	for(; itr != vtNearLinks.end(); )
	{
		if(m_mapClosedLinks.find(*itr) != m_mapClosedLinks.end())
			itr = vtNearLinks.erase(itr);
		/*else if(m_mapOpenLinks.find(*itr) != m_mapOpenLinks.end())
			itr = vtNearLinks.erase(itr);*/
		else
			++itr;
	}

	// todo, sort the near links
	if(vtNearLinks.size() < 1)
	{
		OutputOneRoute(pRoute,0); // todo test		
		Route_vBacktrack(pRoute);
		return;
	}

	// 2. push new link to new route which copy the pre list frome current route
	for(size_t iLink = 1; iLink < vtNearLinks.size(); ++iLink)
	{
		for(size_t i = 0; i < m_vtRearchQuene.size(); ++i)
		{
			if((m_vtRearchQuene[i])&&(m_vtRearchQuene[i]->size() < 1))
			{
				// append the current route
				m_vtRearchQuene[i]->insert(m_vtRearchQuene[i]->end(),\
										pRoute->begin(), pRoute->end() );
				m_vtRearchQuene[i]->push_back(vtNearLinks[iLink]);
				m_mapOpenLinks[ vtNearLinks[iLink] ] = 1;
				break;
			}
		}
	}

	// 3. Get high level next link for this route
	pRoute->push_back(vtNearLinks[0]);
	m_mapOpenLinks[ vtNearLinks[0] ] = 1;

	// 4. check if route  is overbig
	Route_bCheckRoute(pRoute);
}

bool CAStar::Route_bCheckRoute(vtRoute* _pRoute, bool _bIsBack)
{
	bool bRet = false;

	// 1. base distance
	double dTotalRouteDis = 0.0f;
	for(size_t i = 0; i < _pRoute->size(); ++i)
		dTotalRouteDis += (*_pRoute)[i]->m_sRoadLen;
	
	// route is no use, need to callback
	if(dTotalRouteDis > (m_pDistanceForSEpoint * 6))
	{
		bRet = true;		
		if(_bIsBack){
			OutputOneRoute(_pRoute,1); // todo test
			Route_vBacktrack(_pRoute);
		}
	}
	return bRet;
}

double CAStar::Route_dCalcRouteSEDistance(const CDTILineData* _pStartLink, const	CDTILineData* _pEndLink)
{
	double dRet = 0.0f;
	if(_pStartLink && _pEndLink)
	{
		double x1= 0.0f,  y1= 0.0f,  x2= 0.0f,  y2= 0.0f;

		// start link's end point
		if(_pStartLink->m_sDir == 3) // 路形状反方向
		{
			x1 = _pStartLink->m_vtPnts[0];
			y1 =_pStartLink->m_vtPnts[1];
		}else{
			int nLastPntIndex = _pStartLink->m_vtPnts.size() - 1;
			x1 = _pStartLink->m_vtPnts[nLastPntIndex - 1];
			y1 =_pStartLink->m_vtPnts[nLastPntIndex];
		}

		// end link's start point
		if(_pEndLink->m_sDir == 3) // 路形状反方向
		{
			int nLastPntIndex = _pEndLink->m_vtPnts.size() - 1;
			x2 = _pEndLink->m_vtPnts[nLastPntIndex - 1];
			y2 =_pEndLink->m_vtPnts[nLastPntIndex];
		}else{
			x2 = _pEndLink->m_vtPnts[0];
			y2 =_pEndLink->m_vtPnts[1];
		}
		dRet = GetMapDistance(x1,  y1,  x2,  y2);
	}
	return dRet;
}

bool CAStar::Route_vBacktrack(vtRoute* _pRoute)
{
	if(!_pRoute || _pRoute->size() < 1)
		return false;

	// closed the last link
	int m_nID = _pRoute->back()->m_nID;
	m_mapClosedLinks[_pRoute->back()] = 1;
	_pRoute->pop_back();	
			
	if(_pRoute->empty())
		return false;
	
	int nid = _pRoute->back()->m_nID; //test todo

	// backtrack to the pre link, and base it find near links
	int nNode = _pRoute->back()->m_nEndNode;
	vector<CDTILineData*> vtNearLinks;

	// 1. Get next links
	m_pInterceForAStar->ML_GetNearLinks(nNode, vtNearLinks, true);
	
	// filte the closed link
	vector<CDTILineData*>::iterator itr = vtNearLinks.begin();
	for(; itr != vtNearLinks.end(); )
	{
		if(m_mapClosedLinks.find(*itr) != m_mapClosedLinks.end())
			itr = vtNearLinks.erase(itr);
		else if(m_mapOpenLinks.find(*itr) != m_mapOpenLinks.end())
			itr = vtNearLinks.erase(itr);
		else
			++itr;
	}

	// todo, sort the near links
	if(vtNearLinks.size() > 0)
	{
		_pRoute->push_back(vtNearLinks[0]);
		m_mapOpenLinks[ vtNearLinks[0] ] = 1;
		return false;
	}else{
		vector<CDTILineData*>().swap(vtNearLinks);
		return Route_vBacktrack(_pRoute);
	}
}
/////////////BFS end//////////////////////////////

/////////////DFS//////////////////////////////
void CAStar::Route_vDFSRearch(const CDTILineData* _pStartLink, const CDTILineData* _pEndLink)
{
	m_vtRouteTrace.push_back(_pStartLink);
	nSuccessNum = 0;

	Route_bDFS(_pStartLink); //dfs
	
	vtRoute().swap(m_vtRouteTrace);
}

bool CAStar::Route_bDFS(const CDTILineData* _pCurrLink)
{
	// 1.启发，判断退出
	// 1.1 路径列没有对象了
	if(m_vtRouteTrace.empty() || !_pCurrLink)
		return true;

	// 1.2 到达目的地
	if(_pCurrLink == m_pEndLink)
	{
		++nSuccessNum;
		OutputOneRoute(&m_vtRouteTrace,100);
		m_vtRouteTrace.pop_back();
		return true;
	}

	// 1.3. base dist
	if(Route_bCheckRoute(&m_vtRouteTrace, false))
	{
		OutputOneRoute(&m_vtRouteTrace,10);
		m_vtRouteTrace.pop_back();
		return false;
	}

	// 2. 获取新接续
	vector<CDTILineData*> vtNearLinks;
	m_pInterceForAStar->ML_GetNearLinks(_pCurrLink->m_nEndNode, vtNearLinks, true);
	
	sort(vtNearLinks.begin(), vtNearLinks.end(), bLinkCmpByCls);
	vtNearLinks.resize(vtNearLinks.size());

	// 3. push new link to new route which copy the pre list frome current route
	for(size_t iLink = 0; iLink < vtNearLinks.size(); ++iLink)
	{
		// find some one,exit
		if(nSuccessNum > 5)
			return true;

		//  防止环
		if(Route_bIsRing(vtNearLinks[iLink]))
			continue;

		m_vtRouteTrace.push_back(vtNearLinks[iLink]);
		Route_bDFS(vtNearLinks[iLink]);
	}
	m_vtRouteTrace.pop_back();
	return false;
}

bool CAStar::Route_bIsRing(const CDTILineData* _pCurrLink)
{
	for(size_t i = 0; i < m_vtRouteTrace.size(); ++i)
	{
		if(m_vtRouteTrace[i] == _pCurrLink)
			return true;
	}
	return false;
}