#pragma once

#include "Common.h"

#include <fstream>
#include <atlconv.h>
#include <tchar.h>

typedef struct tagRoadLine {
	wstring			wstrRoadName;
	wstring			wstrRoadDirDes;
	int				iRLCD;
	int				iShowLev;
	int				iDrawLev;
	int				iOrder;
	vector<double>	vecPoints;

	int				flag;

	bool operator < (const tagRoadLine& other) const
	{
		return iOrder < other.iOrder;
	}
} ST_ROADLINE, *PST_ROADLINE;

typedef struct tagRoadName {
	size_t			iLineIndex;
	wchar_t			wcText;
	double			dX;
	double			dY;
} ST_ROADNAME, *PST_ROADNAME;


vector<ST_ROADLINE> ReadMifMidFile(char *pMifName, char *pMidName);

void OutputRoadName(char *pMifName, char *pMidName, vector<ST_ROADNAME> &vecRoadName);

bool RoadLineCmp(PST_ROADLINE p1, PST_ROADLINE p2);