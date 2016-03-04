/***************************************************
 * @File: ComDef.h
 * @Description: comm def
 * @Author: changggy 2014-12-18
 * copy all right by AutoPia
 ***************************************************/

#ifndef _COMMON_DEF_H
#define _COMMON_DEF_H
#pragma once

#include <io.h>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "AttrDataProcess.h"
#include "../mapInfo/MFAL.H"
using namespace std;

// ���Mapinfo�����ֶ���Ŀ
#define  MAP_HANDLE_MAPINFO_MAX_SIZE_OF_FIELD 256
class CLinkInfo
{
public:
	CLinkInfo():m_nCls(0),m_dLength(0.0f),m_strMeshLinkID("0"),
			m_strLocIndex("0,0,0"),m_strRoadName("0"),m_strFuncls("0")
	{
	}


public:
	size_t		m_nCls; // ��·�ȼ�
	double		m_dLength; // ��·����

	string		m_strMeshLinkID; // mesh,linkid
	string		m_strLocIndex; // mesh,cls,id
	string		m_strRoadName;
	string		m_strFuncls;
};
 
//����0�������ģ�����1��������
static int IncludeChinese(const char * pSrcstr)
{
   int nRet = 0;
   char * pstr = (char*)pSrcstr;
   char c;
   while(c=*pstr++)
   {
	   //����ַ���λΪ1����һ�ַ���λҲ��1���������ַ�
	   if( (c&0x80) && (*pstr & 0x80) )
	   {
		   nRet = 1;
		   break;
	   }
   }
   return nRet;
}
static void DivString(string& _strSrc, string& _strDivide, vector<string>& _vtStr)
{
	size_t last =0;
	size_t end =0;
	while(string::npos != end)
	{
		end = _strSrc.find_first_of(_strDivide.c_str(),last);
		if(string::npos != end)
		{
			_vtStr.push_back(string (_strSrc, last, end-last));
		}
		else{
			_vtStr.push_back(string(_strSrc, last, _strSrc.length()));
		}
		last = end + 1;
	}
}

#endif