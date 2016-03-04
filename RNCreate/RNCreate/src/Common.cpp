#include "Common.h"
 
#include <time.h>
#include "memory.h"
#include "stdio.h"
#include "malloc.h"
#include "locale.h"
#include "stdlib.h"
#include <complex>   

typedef basic_string<char>::size_type S_T;   
static const S_T npos = -1;   

// 宽字符转UTF8 
string EncodeUtf8(wstring in) 
{ 
	string s(in.length()*3+1,' '); 
	size_t len = ::WideCharToMultiByte(CP_ACP, 0, in.c_str(), in.length(), &s[0], s.length(), NULL, NULL); 
	s.resize(len); 
	return s; 
} 

// UTF8 转宽字符 
wstring DecodeUtf8(string in) 
{ 
	wstring s(in.length(), _T(' '));
	size_t len = ::MultiByteToWideChar(CP_ACP, 0, in.c_str(), in.length(), &s[0], s.length()); 
	s.resize(len); 
	return s; 
} 

// 将一行字符串数据按照分隔符进行分割成若干个独立的字符串
extern vector<string> TokenizeString(const string& src, string tok, bool trim, string null_subst="")   
{   
	vector<string> v;   
	S_T pre_index = 0, index = 0, len = 0;  

	v.clear();

	if( src.empty() || tok.empty() ) 
	{
		return v;   
	}
	
	while( (index = src.find_first_of(tok, pre_index)) != npos )   
	{   
		if( (len = index-pre_index)!=0 )   
		{
			v.push_back(src.substr(pre_index, len));   
		}
		else if(trim==false)   
		{
			v.push_back(null_subst);   
		}
		
		pre_index = index+1;   
	}   
	
	string endstr = src.substr(pre_index);   
	if( trim==false )
	{
		v.push_back( endstr.empty()?null_subst:endstr );   
	}
	else if( !endstr.empty() )
	{
		v.push_back(endstr);   
	}
	
	return v;   
}   

extern vector<wstring> TokenizeWString(const wstring& src, wstring tok, bool trim, wstring null_subst)
{   
	if( src.empty() || tok.empty() ) 
	{
		throw "TokenizeString: empty string\0";   
	}
	
	vector<wstring> v;   
	S_T pre_index = 0, index = 0, len = 0;  
	
	while( (index = src.find_first_of(tok, pre_index)) != npos )   
	{   
		if( (len = index-pre_index)!=0 )   
		{
			v.push_back(src.substr(pre_index, len));   
		}
		else if(trim==false)   
		{
			v.push_back(null_subst);   
		}
		
		pre_index = index+1;   
	}   
	
	wstring endstr = src.substr(pre_index);   
	if( trim==false )
	{
		v.push_back( endstr.empty()?null_subst:endstr );   
	}
	else if( !endstr.empty() )
	{
		v.push_back(endstr);   
	}
	
	return v;   
}   

// 将字符串转换为十六进制数字
bool   StringHexToDec(   string   shex, unsigned short&   idec   )   
{   
	int   iLoop,mid;   
	int   strLen   =   shex.size();   
    
	if(   strLen>8   )   
		return   false;   
    
	mid   =   0;   idec   =   0;   
	for(   iLoop=0; iLoop<strLen; iLoop++   )   
	{   
		if(   shex[iLoop]>='0'&&shex[iLoop]<='9'   )  
		{
			mid   =   shex[iLoop]-'0';   
		}
		else   if(   shex[iLoop]>='a'&&shex[iLoop]<='f'   )  
		{
			mid   =   shex[iLoop]   -'a'   +10;   
		}
		else   if(   shex[iLoop]>='A'&&shex[iLoop]<='F'   )   
		{
			mid   =   shex[iLoop]   -'A'   +10;  
		}
		else   
		{
			return   false;
		}
		
		mid   <<=   ((strLen-iLoop-1)<<2);   
		idec   |=   mid;       
	}   
    
	return   true;   
}   

// 将一行字符串数据按照分隔符进行分割成若干个独立的字符串
extern vector<string> SplitString(const string& src, string delimit,	string null_subst="")   
{   
	if( src.empty() || delimit.empty() )
		throw "SplitString: empty string\0";   
	
	vector<string> v;   
	S_T deli_len = delimit.size();   
	long index = npos, last_search_position = 0;   
	while( (index=src.find(delimit,last_search_position))!=npos )   
	{   
		if(index==last_search_position) 
		{
		}
		else 
		{
			v.push_back( src.substr(last_search_position, index-last_search_position) );   
		}
		
		last_search_position = index + deli_len;   
	}   
	
	string last_one = src.substr(last_search_position);   
	if( !last_one.empty() )
	{
		v.push_back( last_one );   
	}
	
	return v;   
}   

// 将字符串转换为整数
extern int StringToInt( string str)
{
	int i,len = str.size(),num = 0 ;
	
	i = 0 ;
	if (str[ 0 ] == '-' )
	{
		i = 1 ;
	}
	
	while (i < len)
	{
		num = num * 10 + ( int )(str[i] - '0' );
		i++;
	}
	
	if (str[0] == '-' )
	{
		num *= - 1 ;
	}
	
	return num;
}

// 将整数转换为字符串
extern string IntToString( int num)
{
	if (num == 0 )
	{
		return "0" ;
	}
	
	string str = "" ;
	
	int num_ = num > 0 ? num : - 1 * num;
	while (num_)
	{
		str = ( char )(num_ % 10 + 48 ) + str;
		num_ /= 10 ;
	}
	if (num < 0 )
	{
		str = "-" + str;
	}
	
	return str;
}

// 将双精度浮点型转换为字符串
int DoubleToString(double a, int n, string& str)
{
	int iLoop, iData;
	int iArry[50];
	
	if(n < 0)
	{
		return RET_FAILED;
	}
	
	memset(iArry, 0, 50*sizeof(int));
	
	if (a < 1)
	{
		iArry[0] = 0;
	}
	else
	{
		iArry[0] = (int)a;
		a = a - iArry[0];
	}
	
	for (iLoop = 1; iLoop <= n+1; iLoop++)
	{//输出前n位
		a *= 10;
		iData =(int)a;
		iData = iData%10;
		
		iArry[iLoop] = iData;
	}
	
	if( iArry[n+1] >= 5 )
	{
		if( (iArry[n]+1) > 9 )
		{
			iArry[n-1] += 1; 
			iArry[n] = iArry[n]-9;
			if( iArry[n-1] > 9 )
			{
				iArry[n-2] += 1;
				iArry[n-1] = iArry[n-1]-9;
			}
		}
		else
		{
			iArry[n] += 1;
		}
	}
	
	if( iArry[0] == 0 )
	{
		str = "0.";
	}
	else
	{
		str = IntToString(iArry[0]) + ".";
	}
	
	for (iLoop = 1; iLoop < n+1; iLoop++)
	{//输出前n位
		str += iArry[iLoop]+48;
	}
	
	return RET_SUCCESS;
}

// 将字符串转换为双精度浮点型
double StringToDouble( string str)
{
	double num = 0;
	int len = str.size();
	
	num = atof(str.c_str());
	
	return num;
}

// 获取当前时间字符串
void  GetCurTimeString(string& strCurTime)
{
	time_t timep;
	struct tm *CurTime = NULL;
	time(&timep);
	CurTime = localtime(&timep);
	
	strCurTime = IntToString(1900 +CurTime->tm_year)+"-"+IntToString(1+CurTime->tm_mon) +"-"+IntToString(CurTime->tm_mday);
	strCurTime += ("  "+IntToString(CurTime->tm_hour) +":"+IntToString(CurTime->tm_min) +":"+IntToString(CurTime->tm_sec));
}

// 求a/b，输出结果小数点后的前n位
int DivToString(int a, int b, int n, string& str)
{
	int iLoop;
	int iArry[50];

	memset( iArry, 0, 50*sizeof(int));

	if(n < 0)
	{
		return RET_FAILED;
	}

	if (a < b)
	{
		str = "0.";
	}
	else
	{
		str = IntToString(a/b) + ".";
		a = a%b;
	}

	for (iLoop = 0; iLoop <= n; iLoop++)
	{//输出前n位
		a *= 10;
		while (a < b)
		{
			a *= 10;
			iArry[iLoop] = 0;
			iLoop++;
		}

		iArry[iLoop] = a/b;
		a = a%b;
	}

	if( iArry[n] >= 5 )
	{
		if( (iArry[n-1]+1) > 9 )
		{
			iArry[n-2] += 1; 
			iArry[n-1] = iArry[n-1]-9;
			if( iArry[n-2] > 9 )
			{
				iArry[n-2] = 9;
			}
		}
		else
		{
			iArry[n-1] += 1;
		}
	}

	for (iLoop = 0; iLoop < n; iLoop++)
	{//输出前n位
		str += iArry[iLoop]+48;
	}

	return RET_SUCCESS;
}

//把UNICODE文字转换成GB文字
int UNICODEtoGB(char *ucode,char *gbcode,int n)
{
	int			i,ret;
	wchar_t		*wbc;
	char		*mbs;
	if(n%2!=0)
		return 0;
	
	wbc=new wchar_t[n];
	mbs=new char[n];
	memset(wbc,0,n);
	memset(mbs,0,n);
	for(i=0;i<n/2;i++)
	{
		wbc[i]=ucode[2*i]*0x100+ucode[2*i+1];
	}
	
	setlocale(LC_ALL, "");
	ret=wcstombs(mbs,wbc,n );

	if(ret==-1)
	{
		memcpy(gbcode,mbs,n);
		delete(wbc);
		delete(mbs);
		return -1;
	}
	memcpy(gbcode,mbs,n);
	
	delete(mbs);
	delete(wbc);
	return 1;
}



// 根据经纬度计算球面距离
#define C2RAD 				0.017453293		/**<(PI/180) */
#define RADIUS				6378137			/**<radius	 */
double CalcSphLen(double a_dPtX1, double a_dPtY1, double a_dPtX2, double a_dPtY2)
{
	double dTmp, dTmpX, dTmpY;
	dTmp = cos((double)(C2RAD * a_dPtY2));

	dTmpX = (a_dPtX2 - a_dPtX1) * dTmp;
	dTmpY = a_dPtY1 - a_dPtY2;
	return  RADIUS * C2RAD * sqrt(dTmpX * dTmpX  + dTmpY * dTmpY);
}




/************************************************************************/
/* CRNRect                                                              */
/************************************************************************/

CRNRect::CRNRect():minx(0),miny(0),maxx(0),maxy(0)
{
}

CRNRect::CRNRect(double x, double y):minx(x),miny(y),maxx(x),maxy(y)
{
}

CRNRect::CRNRect(double _minx, double _miny, double _maxx, double _maxy):minx(_minx),miny(_miny),maxx(_maxx),maxy(_maxy)
{
}

bool CRNRect::operator == ( const CRNRect& rc )
{
	if ( minx != rc.minx )	return(false);
	if ( maxx != rc.maxx )	return(false);
	if ( miny != rc.miny )	return(false);
	if ( maxy != rc.maxy )	return(false);

	return(true);
}

bool CRNRect::PtInRect( double x, double y ) const
{
	if( x <= minx )		return false;
	else if( x >= maxx )	return false;
	else if( y <= miny )	return false;
	else if( y >= maxy )return false;
	else	return true;
}

void CRNRect::Inflate( double sx, double sy )
{
	minx -= sx;	maxx += sx;
	miny -= sy;	maxy += sy;
}

bool CRNRect::IsRectEmpty()
{
	if ( minx == maxx && miny == maxy )
	{
		return(true);
	}
	return(false);
}

bool CRNRect::IsRectNull()
{
	if( minx == 0 && maxx == 0 &&
		miny == 0 && maxy == 0)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

bool CRNRect::IsIntersect( const CRNRect& rc ) const
{
	if(maxx <= rc.minx)
	{
		return  false;	
	}
	else  if(minx >= rc.maxx)
	{
		return  false;	
	}
	else  if(miny >= rc.maxy)
	{
		return  false;		
	}
	else  if(maxy <= rc.miny)
	{
		return  false;	
	}
	return  true;    
}

void CRNRect::Intersect( const CRNRect& rc )
{
	if( minx < rc.minx )	minx = rc.minx; 
	if( minx > rc.maxx )	minx = rc.maxx; 
	if( maxx < rc.minx )	maxx = rc.minx; 
	if( maxx > rc.maxx )	maxx = rc.maxx; 
	if( miny < rc.miny )	miny = rc.miny; 
	if( miny > rc.maxy )	miny = rc.maxy; 
	if( maxy < rc.miny )	maxy = rc.miny; 
	if( maxy > rc.maxy )	maxy = rc.maxy; 
}


void CRNRect::Union( double x, double y )
{
	if ( minx > x )		minx = x;
	if ( maxx < x )		maxx = x;
	if ( miny > y )		miny = y;
	if ( maxy < y )		maxy = y;
}

void CRNRect::Union( const CRNRect& rc )
{
	if ( minx > rc.minx )	minx = rc.minx;
	if ( maxx < rc.maxx )	maxx = rc.maxx;
	if ( miny > rc.miny )	miny = rc.miny;
	if ( maxy < rc.maxy )	maxy = rc.maxy;
}

void CRNRect::NormalizeRect( void )
{
	double i4Temp;
	if( minx > maxx )
	{
		i4Temp = minx;
		minx = maxx;
		maxx = i4Temp;
	}
	if( miny > maxy )
	{
		i4Temp = maxy;
		maxy = miny;
		miny = i4Temp;
	}
}