#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <string>
#include <vector>
#include <atlconv.h>
#include <tchar.h>
using namespace std;

#define		RET_SUCCESS			(1)
#define		RET_FAILED			(-1)

// 将一行字符串数据按照分隔符进行分割成若干个独立的字符串
vector<string> TokenizeString(const string& src, string tok, bool trim, string null_subst);
vector<wstring> TokenizeWString(const wstring& src, wstring tok, bool trim, wstring null_subst);

// 将一行字符串数据按照分隔符进行分割成若干个独立的字符串
vector<string> SplitString(const string& src, string delimit,string null_subst); 

// 将字符串转换为整数
int StringToInt(string str);
// 将整数转换为字符串
string IntToString( int num);

// 将字符串转换为双精度浮点型
double StringToDouble(string str);
// 将双精度浮点型转换为字符串
int DoubleToString(double a, int n, string& str);

// 求a/b，输出结果小数点后的前n位
int DivToString(int a, int b, int n, string& str);

// 将字符串转换为十六进制数字
bool StringHexToDec(string shex, unsigned short& idec);   

// 获取当前时间戳
void GetCurTimeString(string& strCurTime);


// 根据经纬度计算球面距离
double CalcSphLen(double a_dPtX1, double a_dPtY1, double a_dPtX2, double a_dPtY2);

string EncodeUtf8(wstring in);
wstring DecodeUtf8(string in);

// 基础矩形范围类型
class CRNRect
{
public:
	CRNRect();
	CRNRect(double x, double y);
	CRNRect(double _minx, double _miny, double _maxx, double _maxy);
	bool operator == ( const CRNRect& rc );

	double Width() { return abs(minx-maxx); }
	double Height(){ return abs(miny-maxy); }

	void NormalizeRect( void );

	bool PtInRect( double x, double y ) const;
	void Inflate( double sx, double sy );
	bool IsRectEmpty();
	bool IsRectNull();

	bool IsIntersect( const CRNRect& rc ) const;
	void Intersect( const CRNRect& rc );

	void Union( double x, double y );
	void Union( const CRNRect& rc );

	double minx,miny,maxx,maxy;
};


#endif /* end if COMMON_FUNCTION_H */
