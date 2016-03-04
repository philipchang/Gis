#ifndef COMMON_FUNCTION_H
#define COMMON_FUNCTION_H

#include <string>
#include <vector>
#include <atlconv.h>
#include <tchar.h>
using namespace std;

#define		RET_SUCCESS			(1)
#define		RET_FAILED			(-1)

// ��һ���ַ������ݰ��շָ������зָ�����ɸ��������ַ���
vector<string> TokenizeString(const string& src, string tok, bool trim, string null_subst);
vector<wstring> TokenizeWString(const wstring& src, wstring tok, bool trim, wstring null_subst);

// ��һ���ַ������ݰ��շָ������зָ�����ɸ��������ַ���
vector<string> SplitString(const string& src, string delimit,string null_subst); 

// ���ַ���ת��Ϊ����
int StringToInt(string str);
// ������ת��Ϊ�ַ���
string IntToString( int num);

// ���ַ���ת��Ϊ˫���ȸ�����
double StringToDouble(string str);
// ��˫���ȸ�����ת��Ϊ�ַ���
int DoubleToString(double a, int n, string& str);

// ��a/b��������С������ǰnλ
int DivToString(int a, int b, int n, string& str);

// ���ַ���ת��Ϊʮ����������
bool StringHexToDec(string shex, unsigned short& idec);   

// ��ȡ��ǰʱ���
void GetCurTimeString(string& strCurTime);


// ���ݾ�γ�ȼ����������
double CalcSphLen(double a_dPtX1, double a_dPtY1, double a_dPtX2, double a_dPtY2);

string EncodeUtf8(wstring in);
wstring DecodeUtf8(string in);

// �������η�Χ����
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
