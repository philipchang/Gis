#include "DataAcc.h"





/************************************************************************/
/* MifMid读取                                                           */
/************************************************************************/

int ReadMidLineFile(char* strLine, vector<string>& vStrLst)
{
	size_t iLoop;
	string strTok = ",";
	string strDrmLnk = "";

	if( !vStrLst.empty())
	{
		vStrLst.clear();
	}

	vStrLst = TokenizeString(strLine, strTok ,true,""); 
	if( vStrLst.empty() )
	{
		return RET_FAILED;
	}

	// 处理引号间逗号产生的问题
	for(iLoop =0; iLoop<vStrLst.size();iLoop++)   
	{   
		std::string::size_type  iIdx;
		iIdx = vStrLst[iLoop].find("\"");
		if ( iIdx != std::string::npos )
		{
			iIdx = vStrLst[iLoop].find("\"", iIdx+1);
			if (iIdx == std::string::npos && iLoop < vStrLst.size()-1) {
				vStrLst[iLoop] += ",";
				vStrLst[iLoop] += vStrLst[iLoop+1];
				for (int iLoopTmp = iLoop+1; iLoopTmp < vStrLst.size()-1; iLoopTmp++)
				{
					vStrLst[iLoopTmp] = vStrLst[iLoopTmp+1];
				}
				vStrLst.pop_back();
				iLoop--;
			}
		}
	}

	for(iLoop =0; iLoop<vStrLst.size();iLoop++)   
	{   
		std::string::size_type  iIdx;
		iIdx = vStrLst[iLoop].find("\"");
		while( iIdx != std::string::npos )
		{
			vStrLst[iLoop].erase(iIdx, 1);
			iIdx = vStrLst[iLoop].find("\"");
		}
	}

	return RET_SUCCESS;
}

int ReadMifOneElement(ifstream& fMifFile, vector<pair<double, double> >& vCoorXYLst)
{
	int iLineLoop, iPntLoop;
	int iLineNum = 0;
	int iPntNum = 0;
	string strTok = " ";  
	string strPline = "Pline";
	string strLine = "Line";
	string strMultiple = "Multiple";
	double dX, dY;
	char  acLine[1024];
	vector<string> vFirstLineOfEle;

	vCoorXYLst.clear();

	do {
		memset(acLine, 0, 1024 );
		fMifFile.getline(acLine, 1024);
	} while( strcmp(acLine,"") == 0);

	vFirstLineOfEle.clear();
	vFirstLineOfEle = SplitString(acLine, strTok, "");   

	if( 0 == vFirstLineOfEle.size())
	{
		//log
		return RET_FAILED;
	}

	// 修改适应不同的mif样式表述 gaojian 2014-01-17
	while (vFirstLineOfEle[0] != strPline && vFirstLineOfEle[0] != strLine)
	{
		if (fMifFile.eof()) {
			return RET_FAILED;
		}

		do {
			memset(acLine, 0, 1024 );
			fMifFile.getline(acLine, 1024);
			if (fMifFile.eof()) {
				return RET_FAILED;
			}
		} while( strcmp(acLine,"") == 0);

		vFirstLineOfEle.clear();
		vFirstLineOfEle = SplitString(acLine, strTok, "");   

		if( 0 == vFirstLineOfEle.size())
		{
			//log
			return RET_FAILED;
		}
	}

	if( vFirstLineOfEle[0] == strPline )
	{
		if( vFirstLineOfEle[1] == strMultiple)
		{
			iLineNum = StringToInt(vFirstLineOfEle[2]);

			for( iLineLoop = 0; iLineLoop < iLineNum; iLineLoop++ )
			{
				memset(acLine, 0, 1024 );
				fMifFile.getline(acLine, 1024);

				vFirstLineOfEle.clear();
				vFirstLineOfEle = SplitString(acLine, strTok, "");
				iPntNum = StringToInt(vFirstLineOfEle[0]);
				for( iPntLoop = 0; iPntLoop < iPntNum; iPntLoop++ )
				{
					memset(acLine, 0, 1024 );
					fMifFile.getline(acLine, 1024);

					vFirstLineOfEle.clear();
					vFirstLineOfEle = SplitString(acLine, strTok, "");   

					dX = StringToDouble(vFirstLineOfEle[0]);
					dY = StringToDouble(vFirstLineOfEle[1]);
					vCoorXYLst.push_back(make_pair(dX,dY));				
				}
			}
		}
		else
		{
			iPntNum = StringToInt(vFirstLineOfEle[1]);

			for( iPntLoop = 0; iPntLoop < iPntNum; iPntLoop++ )
			{
				memset(acLine, 0, 1024 );
				fMifFile.getline(acLine, 1024);

				vFirstLineOfEle.clear();
				vFirstLineOfEle = SplitString(acLine, strTok, "");   

				dX = StringToDouble(vFirstLineOfEle[0]);
				dY = StringToDouble(vFirstLineOfEle[1]);
				vCoorXYLst.push_back(make_pair(dX,dY));				
			}
		}
	}
	else if( vFirstLineOfEle[0] == strLine )
	{
		dX = StringToDouble(vFirstLineOfEle[1]);
		dY = StringToDouble(vFirstLineOfEle[2]);
		vCoorXYLst.push_back(make_pair(dX,dY));

		dX = StringToDouble(vFirstLineOfEle[3]);
		dY = StringToDouble(vFirstLineOfEle[4]);
		vCoorXYLst.push_back(make_pair(dX,dY));
	}
	else
	{
		//log
		return RET_FAILED;
	}

	fMifFile.getline(acLine, 1024);


	return RET_SUCCESS;
}

// 读取文件
vector<ST_ROADLINE> ReadMifMidFile(char *pMifName, char *pMidName)
{
	char acLine[1024];
	vector<string> vMidStrLst;
	vector<pair<double, double> > vCoorXYLst;

	vector<ST_ROADLINE> vecRoadLineLst;


	// 打开文件
	ifstream fMifFile(pMifName, ios::in);
	ifstream fMidFile(pMidName, ios::in);

	if( !fMifFile.is_open() || !fMidFile.is_open() ) {
		return vecRoadLineLst;
	}

	// MIF文件偏移到数据部分
	do {
		memset(acLine, 0, 1024 );
		fMifFile.getline(acLine, 1024);
	} while( strcmp(acLine,"Data") != 0);

	// 读取MIF/MID文件
	do {
		memset(acLine, 0, 1024 );
		fMidFile.getline(acLine, 1024);

		// 读取一行MID文件
		vMidStrLst.clear();
		if( RET_FAILED == ReadMidLineFile( acLine, vMidStrLst)) {
			break;
		}

		// 读取一行MIF文件
		vCoorXYLst.clear();
		if( RET_FAILED == ReadMifOneElement( fMifFile, vCoorXYLst)) {
			break;
		}

		// 生成道路结构
		ST_ROADLINE stLine;
		stLine.wstrRoadDirDes = DecodeUtf8(vMidStrLst[8]);
		stLine.iRLCD = StringToInt(vMidStrLst[9]);
		stLine.wstrRoadName = DecodeUtf8(vMidStrLst[6]);
		stLine.iShowLev = StringToInt(vMidStrLst[11]);
		stLine.iDrawLev = StringToInt(vMidStrLst[12]);
		stLine.iOrder = StringToInt(vMidStrLst[10]);
		

		vector<pair<double, double> >::iterator iter = vCoorXYLst.begin();
		for (; iter != vCoorXYLst.end(); iter++)
		{
			stLine.vecPoints.push_back(iter->first);
			stLine.vecPoints.push_back(iter->second);
		}

		stLine.flag = 0;

		vecRoadLineLst.push_back(stLine);

	} while(true);

	return vecRoadLineLst;
}



/************************************************************************/
/* MifMid输出                                                           */
/************************************************************************/

void OutputRoadName(char *pMifName, char *pMidName, vector<ST_ROADNAME> &vecRoadName)
{
	FILE * pPointMifOut = fopen(pMifName,"wb+");
	FILE * pPointMidOut = fopen(pMidName,"wb+");
	if (pMifName == NULL || pPointMidOut == NULL) {
		return;
	}

	fprintf(pPointMifOut,"Version 300");
	fprintf(pPointMifOut,"Charset \"WindowsSimpChinese\"\r\n");
	fprintf(pPointMifOut,"Delimiter \",\"\r\n");
	fprintf(pPointMifOut,"CoordSys Earth Projection 1, 0\r\n");
	fprintf(pPointMifOut,"Columns 1\r\n");
	fprintf(pPointMifOut,"Name Char(64)\r\n");
	fprintf(pPointMifOut,"Data\r\n");

	//输出
	int mergyStart = 0;
	int mergyEnd = 0;
	int Len = vecRoadName.size();
	for(int i = 1; i < Len; ++i)
	{	
		if(i != (Len - 1))
		{
			if(	vecRoadName[i].iLineIndex == vecRoadName[mergyStart].iLineIndex)
			{
				mergyEnd = i;
				continue;
			}
		}
		else
		{
			mergyEnd = i; // the laset one
		}
		
		wstring wstr;
		fprintf(pPointMifOut,"Pline %d\r\n", (mergyEnd - mergyStart + 1));
		for(int j = mergyStart;  j <= mergyEnd; ++j)
		{
			fprintf(pPointMifOut,"%0.6f %0.6f\r\n", vecRoadName[j].dX, vecRoadName[j].dY);
			wstr += vecRoadName[j].wcText;
		}
		fprintf(pPointMifOut,"    Pen (1,2,0)");

		string str = EncodeUtf8(wstr);
		fprintf(pPointMidOut, "\"%s\"\r\n", str.c_str());

		mergyEnd = i;
		mergyStart = i; // reset
	}

	fclose(pPointMifOut);
	fclose(pPointMidOut);
}

bool RoadLineCmp(PST_ROADLINE p1, PST_ROADLINE p2)
{
	bool bRet = false;
	if(p1 && p2) {	
		bRet = p1->operator<(*p2);
	}	 
	return bRet;
}

