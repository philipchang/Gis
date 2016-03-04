// OffsetGps.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GpsOffset.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

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
void PushItem(string& strSrc, ofstream& fout)
{
	if( strSrc.empty() )
		return;

	vector<string> vtstrs;
	string strBreak(",");
	DivString(strSrc, strBreak,vtstrs);	
		
	double marslon = 0;
	double marslat = 0;
	double gps_x = atof(vtstrs[0].c_str());
	double gps_y = atof(vtstrs[1].c_str());

	gps_transform( gps_y, gps_x, marslat, marslon);

	char strBuf[128] = {0};
	sprintf(strBuf, "%f,%f\n", marslon, marslat);
	fout.write(strBuf, strlen(strBuf));
}


bool InitialData( char* pDataTabName )
{
	bool bRet = false;
	ifstream  fin( pDataTabName );
	
	string strOutput(pDataTabName);
	strOutput +=  "out.csv";
	ofstream  fout( strOutput.c_str() );
	if(fin && fout)
	{
		string strLine;
		getline(fin,strLine); // del title
		fout.write("x,y\n", 4);

		if(1)
		//if( 0 == strLine.compare("¿‡–Õ,√˚≥∆,Display_X,Display_Y,ShowLev,SubCls"))
		{
			bRet = true;
			while(getline(fin,strLine))
			{			
				PushItem( strLine, fout );
			}
		} else {
			cout<<"\n ******************************\ncur POI file format error\n ******************************\n";
		}
		
	}
	fout.close();
	fin.close();
	return bRet;
}



int _tmain(int argc, _TCHAR* argv[])
{

	double marslon = 0;
	double marslat = 0;
	double gps_x = 116.467834;
	double gps_y = 39.955647;

	gps_transform( gps_y, gps_x, marslat, marslon);

	printf( "%f,%f\n", marslon, marslat);

#define pi  3.14f;
	double testdouble = (double)pi;

	printf("%.20f\n", testdouble);

	InitialData("gps.csv");
	return 0;
}

