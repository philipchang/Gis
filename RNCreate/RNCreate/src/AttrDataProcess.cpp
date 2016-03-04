// AttrDataProcess.cpp: implementation of the CAttrDataProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "AttrDataProcess.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAttrDataProcess::CAttrDataProcess()
{

}

CAttrDataProcess::~CAttrDataProcess()
{

}

int  CAttrDataProcess::SplitAttrData(char *pAttrData,VCHARARRAY* pFieldValue)
{
	int m_nField = 0;
	string m_strFieldVaule=string(pAttrData);   
	
    int length = strlen(pAttrData);
    if (length == 0)
        return 0;	
    string field;
	
    int cur_pos = 0;
    int next_pos = 0;
    do {
        // skip spaces
        for (; isspace(pAttrData[cur_pos]); cur_pos++);
        
        if (pAttrData[cur_pos] == '"')  
            next_pos = ExtractstringField(m_strFieldVaule, field, cur_pos);
        else
            next_pos = ExtractNumericField(m_strFieldVaule, field, cur_pos);
		
		pFieldValue->push_back(field);
		
        m_nField++;		
        cur_pos = next_pos + 1; // skip current ',' and start looking for the next
		
    } while (cur_pos < length);
	
	return m_nField;	
}



bool CAttrDataProcess::isspace(char c)
{
	return (c==' ') ? true : false;
}

int CAttrDataProcess::ExtractstringField(string& source, string& field, int start_pos)
{
    field = "";
	
    int cur_pos = 0;
    int line_length = source.length();
	
    start_pos++;   // skip first '"'
	
    // find next '"'
    cur_pos = source.find_first_of("\"", start_pos);
    if (cur_pos < 0 )
    {
        cur_pos = line_length;
    }
	
    field = string(source, start_pos, cur_pos-start_pos);
	
    // find next ','
    cur_pos = source.find_first_of("\t", cur_pos);
    if (cur_pos < 0 )
    {
        cur_pos = line_length;
    }
	
    return cur_pos;
}


int CAttrDataProcess::ExtractNumericField(string& source, string& field, int start_pos)
{
    field = "";
	
    int cur_pos = 0;
    int line_length = source.length();
    
    // find first ','
    cur_pos = source.find_first_of("\t", start_pos);
    if (cur_pos < 0 )
    {
        cur_pos = line_length;
    }
    
    field = string(source, start_pos, cur_pos-start_pos);
	
    return cur_pos;
}

