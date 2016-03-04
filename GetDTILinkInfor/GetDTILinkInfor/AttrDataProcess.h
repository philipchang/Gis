// AttrDataProcess.h: interface for the CAttrDataProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATTRDATAPROCESS_H__8CE74043_F9BA_4D84_92B5_0E0B45806273__INCLUDED_)
#define AFX_ATTRDATAPROCESS_H__8CE74043_F9BA_4D84_92B5_0E0B45806273__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////
using namespace std;

typedef vector<string> VCHARARRAY;
class CAttrDataProcess  
{
public:
	CAttrDataProcess();
	virtual ~CAttrDataProcess();

	static int SplitAttrData(char *pAttrData,VCHARARRAY* pFieldValue);
	static int ExtractstringField(string& source, string& field, int start_pos);
	static int ExtractNumericField(string& source, string& field, int istart_pos);
	static bool isspace(char c);

};

#endif // !defined(AFX_ATTRDATAPROCESS_H__8CE74043_F9BA_4D84_92B5_0E0B45806273__INCLUDED_)
