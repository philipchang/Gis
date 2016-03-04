#ifndef _CNEW_SIM_H
#define _CNEW_SIM_H

#pragma once
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "GlobalMercator.h"
using namespace std;

static int SelfITDist[19] = {
    26,     // 0
    26,     // 1
    26,     // 2
    26,     // 3
    26,     // 4
    26,     // 5
    26,     // 6
    26,     // 7
    26,     // 8
    26,     // 9
    26,     // 10
    26,     // 11
    26,     // 12
    6,     // 13
    6,     // 14
    4,     // 15
    4,     // 16
    4,     // 17
    4      // 18
};

#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

template <class TYPE>
class TSimpDP;

enum
{
	ROUNDS_CAP = 0,
	SQUARE_CAP = 1
};

template <class TYPE>
class TLineEX
{
public:
	TLineEX();
	~TLineEX();

	void MoveTo(TYPE X1, TYPE Y1);
	void LineTo(TYPE X2, TYPE Y2);
	void Finish();
	void SelfIT(char *Work, long cnt);

	void SetLnWidth(TYPE nWidth);
	void SetLineCap(long nLnCap);
	void SetJoinCap(long nJnCap);

protected:
	void InitEx();
	void NextEx();
	void InsertLPnt(TYPE X1, TYPE Y1);
	void InsertRPnt(TYPE X1, TYPE Y1);

	// 计算线段之间圆角连接曲线
	class RCAP
	{
	public:
		TYPE cx, cy;		// cx, cy 圆心坐标
		TYPE x1, y1;		// x1, y1 起点坐标
		TYPE x2, y2;		// x2, y2 终点坐标
		TYPE ba, ea;		// ba, ea 起点角度，终点角度
	//	TYPE da, ra;		// da, ra 角度步长，圆角半径
	};
	void RoundLCap(RCAP *rcap);
	void RoundRCap(RCAP *rcap);

	static void LineTranslate(TYPE *LineC, TYPE *LineL, TYPE *LineR, TYPE Dist);
	// 两条线段夹角转向，X1,Y1->X2,Y2->X3,Y2；> 0 左转、逆时针；< 0 右转、顺时针
	static TYPE TurningOfLine(TYPE *LineT);

	enum
	{
		DONT_INTERSECT = 0,
		LINE_INTERSECT = 1,
		LINE_COLLINEAR = 2
	};
	static long LineIntersect(TYPE *LineA, TYPE *LineB, TYPE *Point);

public:
	long  m_LnCap;
	long  m_JnCap;

	TYPE  m_Width;
	TYPE  m_Steps;
	TYPE  m_LineC[6];
	TYPE  m_LineL[4];
	TYPE  m_LineR[4];
	TYPE* m_LastC;

	TYPE  m_vPnts[8];
	long  m_bPnts;

	TYPE* m_BuffL;
	long  m_MaxsL;
	long  m_SizeL;
	TYPE* m_BuffR;
	long  m_MaxsR;
	long  m_SizeR;
};

template <class TYPE>
TLineEX<TYPE>::TLineEX()
{
	m_LnCap = SQUARE_CAP;
	m_JnCap = SQUARE_CAP;
	m_Width = 2.5;
	m_Steps = 0.4;
	m_LastC = m_LineC;

	m_BuffL = NULL;
	m_MaxsL = 0;
	m_SizeL = 0;
	m_BuffR = NULL;
	m_MaxsR = 0;
	m_SizeR = 0;

	m_bPnts = 0;
}

template <class TYPE>
TLineEX<TYPE>::~TLineEX()
{
	if (m_BuffL != NULL)
		free(m_BuffL);
	if (m_BuffR != NULL)
		free(m_BuffR);
}

template <class TYPE>
void TLineEX<TYPE>::MoveTo(TYPE X1, TYPE Y1)
{
	Finish();
	m_LineC[0] = X1;
	m_LineC[1] = Y1;
}

template <class TYPE>
void TLineEX<TYPE>::LineTo(TYPE X2, TYPE Y2)
{
	if (X2 == m_LastC[0] && Y2 == m_LastC[1])
		return;

	m_LastC[2] = X2;
	m_LastC[3] = Y2;

	if (m_LastC == m_LineC)
		InitEx(), m_LastC += 2;
	else
		NextEx();
}

template <class TYPE>
void TLineEX<TYPE>::Finish()
{
	if (m_LastC > m_LineC)
	{
		TYPE dx = m_LineC[2] - m_LineC[0];
		TYPE dy = m_LineC[3] - m_LineC[1];
		TYPE dl = sqrt(dx * dx + dy * dy);

		if (dl < m_Width && dl > 0)
		{
			m_bPnts |= 2;
			m_vPnts[4] = m_LineC[0];
			m_vPnts[5] = m_LineC[1];

			m_vPnts[6] = m_LineC[0] + m_Width / dl * dx;
			m_vPnts[7] = m_LineC[1] + m_Width / dl * dy;
		}

		m_LastC = m_LineC;

		InsertRPnt(m_LineR[2], m_LineR[3]);
		InsertLPnt(m_LineL[2], m_LineL[3]);
	}
}

template <class TYPE>
void TLineEX<TYPE>::SelfIT(char *Work, long cnt)
{
	if (m_SizeL < 4 ||
		m_SizeR < 4)
		return;

	typedef TSimpDP<TYPE> CSimp;

	TYPE b_LineL[4], b_LineR[4];
	TYPE e_LineL[4], e_LineR[4];

	TYPE b_BuffL[4], b_BuffR[4];
	TYPE e_BuffL[4], e_BuffR[4];

	if (m_bPnts & 1)
	{
		LineTranslate(m_vPnts + 0, b_LineL, b_LineR, m_Width);

		b_BuffL[0] = m_BuffL[0];
		b_BuffL[1] = m_BuffL[1];
		b_BuffL[2] = m_BuffL[2];
		b_BuffL[3] = m_BuffL[3];
		b_BuffR[0] = m_BuffR[0];
		b_BuffR[1] = m_BuffR[1];
		b_BuffR[2] = m_BuffR[2];
		b_BuffR[3] = m_BuffR[3];

		m_BuffL[0] = b_LineL[0];
		m_BuffL[1] = b_LineL[1];
		b_LineL[2] = m_BuffL[2];
		b_LineL[3] = m_BuffL[3];
		m_BuffR[0] = b_LineR[0];
		m_BuffR[1] = b_LineR[1];
		b_LineR[2] = m_BuffR[2];
		b_LineR[3] = m_BuffR[3];
	}
	if (m_bPnts & 2)
	{
		LineTranslate(m_vPnts + 4, e_LineL, e_LineR, m_Width);

		long lastl = m_SizeL * 2;
		long lastr = m_SizeR * 2;
		e_BuffL[0] = m_BuffL[lastl - 4];
		e_BuffL[1] = m_BuffL[lastl - 3];
		e_BuffL[2] = m_BuffL[lastl - 2];
		e_BuffL[3] = m_BuffL[lastl - 1];
		e_BuffR[0] = m_BuffR[lastr - 4];
		e_BuffR[1] = m_BuffR[lastr - 3];
		e_BuffR[2] = m_BuffR[lastr - 2];
		e_BuffR[3] = m_BuffR[lastr - 1];

		e_LineL[0] = m_BuffL[lastl - 4];
		e_LineL[1] = m_BuffL[lastl - 3];
		m_BuffL[lastl - 2] = e_LineL[2];
		m_BuffL[lastl - 1] = e_LineL[3];

		e_LineR[0] = m_BuffR[lastr - 4];
		e_LineR[1] = m_BuffR[lastr - 3];
		m_BuffR[lastr - 2] = e_LineR[2];
		m_BuffR[lastr - 1] = e_LineR[3];
	}

	m_SizeL = CSimp::SelfIT(m_BuffL, m_SizeL, Work, cnt);
	m_SizeR = CSimp::SelfIT(m_BuffR, m_SizeR, Work, cnt);

	{
		TYPE *begs = m_BuffL;
		TYPE *ends = m_BuffL + m_SizeL * 2;
		if (m_bPnts & 1)
		{
			if (fabs(m_BuffL[0] - b_LineL[0]) < 1E-6 && fabs(m_BuffL[1] - b_LineL[1]) < 1E-6 &&
				fabs(m_BuffL[2] - b_LineL[2]) < 1E-6 && fabs(m_BuffL[3] - b_LineL[3]) < 1E-6)
				m_BuffL[0] = b_BuffL[0], m_BuffL[1] = b_BuffL[1];
			else
				begs += 2;
		}
		if (m_bPnts & 2)
		{
			long lastl = m_SizeL * 2;
			if (fabs(m_BuffL[lastl - 4] - e_LineL[0]) < 1E-6 && fabs(m_BuffL[lastl - 3] - e_LineL[1]) < 1E-6 &&
				fabs(m_BuffL[lastl - 2] - e_LineL[2]) < 1E-6 && fabs(m_BuffL[lastl - 1] - e_LineL[3]) < 1E-6)
				m_BuffL[lastl - 2] = e_BuffL[2], m_BuffL[lastl - 1] = e_BuffL[3];
			else
				ends -= 2;
		}

		m_SizeL = (ends - begs) / 2;
		if (begs != m_BuffL)
		{
			TYPE *curs = m_BuffL;
			while(begs < ends)
				*curs++ = *begs++;
		}
	}

	{
		TYPE *begs = m_BuffR;
		TYPE *ends = m_BuffR + m_SizeR * 2;
		if (m_bPnts & 1)
		{
			if (fabs(m_BuffR[0] - b_LineL[0]) < 1E-6 && fabs(m_BuffR[1] - b_LineL[1]) < 1E-6 &&
				fabs(m_BuffR[2] - b_LineL[2]) < 1E-6 && fabs(m_BuffR[3] - b_LineL[3]) < 1E-6)
				m_BuffR[0] = b_BuffR[0], m_BuffR[1] = b_BuffR[1];
			else
				begs += 2;
		}
		if (m_bPnts & 2)
		{
			long lastr = m_SizeR * 2;
			if (fabs(m_BuffR[lastr - 4] - e_LineR[0]) < 1E-6 && fabs(m_BuffR[lastr - 3] - e_LineR[1]) < 1E-6 &&
				fabs(m_BuffR[lastr - 2] - e_LineR[2]) < 1E-6 && fabs(m_BuffR[lastr - 1] - e_LineR[3]) < 1E-6)
				m_BuffR[lastr - 2] = e_BuffR[2], m_BuffR[lastr - 1] = e_BuffR[3];
			else
				ends -= 2;
		}

		m_SizeR = (ends - begs) / 2;
		if (begs != m_BuffR)
		{
			TYPE *curs = m_BuffR;
			while(begs < ends)
				*curs++ = *begs++;
		}
	}

	m_bPnts = 0;
}

template <class TYPE>
inline void TLineEX<TYPE>::SetLnWidth(TYPE nWidth)
{
	m_Width = nWidth;
	m_Steps = 0.5;
}

template <class TYPE>
inline void TLineEX<TYPE>::SetLineCap(long nLnCap)
{
	m_LnCap = nLnCap;
}

template <class TYPE>
inline void TLineEX<TYPE>::SetJoinCap(long nJnCap)
{
	m_JnCap = nJnCap;
}

template <class TYPE>
void TLineEX<TYPE>::InitEx()
{
	m_SizeL = 0;
	m_SizeR = 0;
	m_bPnts = 0;

	LineTranslate(m_LineC + 0, m_LineL, m_LineR, m_Width);

	InsertRPnt(m_LineR[0], m_LineR[1]);
	InsertLPnt(m_LineL[0], m_LineL[1]);

	TYPE dx = m_LineC[2] - m_LineC[0];
	TYPE dy = m_LineC[3] - m_LineC[1];
	TYPE dl = sqrt(dx * dx + dy * dy);

	if (dl < m_Width)
	{
		m_bPnts |= 1;
		m_vPnts[0] = m_LineC[2] - m_Width / dl * dx;
		m_vPnts[1] = m_LineC[3] - m_Width / dl * dy;

		m_vPnts[2] = m_LineC[2];
		m_vPnts[3] = m_LineC[3];
	}
}

template <class TYPE>
void TLineEX<TYPE>::NextEx()
{
	TYPE t_LineL[4], t_LineR[4];
	LineTranslate(m_LineC + 2, t_LineL, t_LineR, m_Width);

	TYPE turn = TurningOfLine(m_LineC);
	if (turn > 0)
	{
		TYPE t_TempI[2];
		if (LineIntersect(m_LineL, t_LineL, t_TempI) == LINE_INTERSECT)
		{
			t_LineL[0] = t_TempI[0];
			t_LineL[1] = t_TempI[1];
			InsertLPnt(t_LineL[0], t_LineL[1]);
		}
		else
		{
			InsertLPnt(m_LineL[2], m_LineL[3]);
			InsertLPnt(t_LineL[0], t_LineL[1]);
		}

		{
			InsertRPnt(m_LineR[2], m_LineR[3]);
		}
		if (m_JnCap == ROUNDS_CAP)
		{
			RCAP rcap;
			rcap.cx = m_LineC[2];
			rcap.cy = m_LineC[3];
			rcap.x1 = m_LineR[2];
			rcap.y1 = m_LineR[3];
			rcap.x2 = t_LineR[0];
			rcap.y2 = t_LineR[1];
			rcap.ba = atan2(m_LineR[3] - m_LineC[3], m_LineR[2] - m_LineC[2]);
			rcap.ea = atan2(t_LineR[1] - m_LineC[3], t_LineR[0] - m_LineC[2]);
			RoundRCap(&rcap);
		}
		else
		{
			InsertRPnt(t_LineR[0], t_LineR[1]);
		}
	}
	else if (turn < 0)
	{
		TYPE t_TempI[2];
		if (LineIntersect(m_LineR, t_LineR, t_TempI) == LINE_INTERSECT)
		{
			t_LineR[0] = t_TempI[0];
			t_LineR[1] = t_TempI[1];
			InsertRPnt(t_LineR[0], t_LineR[1]);
		}
		else
		{
			InsertRPnt(m_LineR[2], m_LineR[3]);
			InsertRPnt(t_LineR[0], t_LineR[1]);
		}

		{
			InsertLPnt(m_LineL[2], m_LineL[3]);
		}
		if (m_JnCap == ROUNDS_CAP)
		{
			RCAP rcap;
			rcap.cx = m_LineC[2];
			rcap.cy = m_LineC[3];
			rcap.x1 = m_LineL[2];
			rcap.y1 = m_LineL[3];
			rcap.x2 = t_LineL[0];
			rcap.y2 = t_LineL[1];
			rcap.ba = atan2(m_LineL[3] - m_LineC[3], m_LineL[2] - m_LineC[2]);
			rcap.ea = atan2(t_LineL[1] - m_LineC[3], t_LineL[0] - m_LineC[2]);
			RoundLCap(&rcap);
		}
		else
		{
			InsertLPnt(t_LineL[0], t_LineL[1]);
		}
	}
	else
	{
		goto _Update_Line_;
	}

	m_LineL[0] = t_LineL[0];
	m_LineL[1] = t_LineL[1];
	m_LineR[0] = t_LineR[0];
	m_LineR[1] = t_LineR[1];
	m_LineC[0] = m_LineC[2];
	m_LineC[1] = m_LineC[3];

_Update_Line_:
	m_LineL[2] = t_LineL[2];
	m_LineL[3] = t_LineL[3];
	m_LineR[2] = t_LineR[2];
	m_LineR[3] = t_LineR[3];
	m_LineC[2] = m_LineC[4];
	m_LineC[3] = m_LineC[5];
}

template <class TYPE>
void TLineEX<TYPE>::InsertLPnt(TYPE X1, TYPE Y1)
{
	if (m_SizeL >= m_MaxsL)
	{
		long  maxs = m_MaxsL + 0x1000;
		TYPE *buff = (TYPE *)realloc(m_BuffL, maxs * sizeof(TYPE) * 2);
		if (buff == NULL)
			return;
		m_MaxsL = maxs;
		m_BuffL = buff;
	}

	TYPE *lpnt = m_BuffL + m_SizeL++ * 2;
	*lpnt++ = X1;
	*lpnt++ = Y1;
}

template <class TYPE>
void TLineEX<TYPE>::InsertRPnt(TYPE X1, TYPE Y1)
{
	if (m_SizeR >= m_MaxsR)
	{
		long  maxs = m_MaxsR + 1024 * 4;
		TYPE *buff = (TYPE *)realloc(m_BuffR, maxs * sizeof(TYPE) * 2);
		if (buff == NULL)
			return;
		m_MaxsR = maxs;
		m_BuffR = buff;
	}
	
	TYPE *rpnt = m_BuffR + m_SizeR++ * 2;
	*rpnt++ = X1;
	*rpnt++ = Y1;
}

template <class TYPE>
void TLineEX<TYPE>::RoundLCap(RCAP *rcap)
{
	TYPE cx = rcap->cx;
	TYPE cy = rcap->cy;
	TYPE x1 = rcap->x1;
	TYPE y1 = rcap->y1;
	TYPE ba = rcap->ba;
	TYPE ea = rcap->ea;
	TYPE da = m_Steps;
	TYPE ra = m_Width;

	if (ea > ba)
		ea = ea - PI * 2.0;

	TYPE ta = ba - ea;
	long sa = (long)(ta / da + 0.8);
	TYPE aa = ta / sa + 1E-4;

	while((ba -= aa) > ea)
	{
		TYPE dx = cos(ba) * ra;
		TYPE dy = sin(ba) * ra;

		TYPE x2 = cx + dx;
		TYPE y2 = cy + dy;

		InsertLPnt(x2, y2);
		x1 = x2;
		y1 = y2;
	}
	{
		TYPE x2 = rcap->x2;
		TYPE y2 = rcap->y2;
		InsertLPnt(x2, y2);
	}
}

template <class TYPE>
void TLineEX<TYPE>::RoundRCap(RCAP *rcap)
{
	TYPE cx = rcap->cx;
	TYPE cy = rcap->cy;
	TYPE x1 = rcap->x1;
	TYPE y1 = rcap->y1;
	TYPE ba = rcap->ba;
	TYPE ea = rcap->ea;
	TYPE da = m_Steps;
	TYPE ra = m_Width;

	if (ea < ba)
		ea = ea + PI * 2.0;

	TYPE ta = ea - ba;
	long sa = (long)(ta / da + 0.8);
	TYPE aa = ta / sa + 1E-4;

	while((ba += aa) < ea)
	{
		TYPE dx = cos(ba) * ra;
		TYPE dy = sin(ba) * ra;

		TYPE x2 = cx + dx;
		TYPE y2 = cy + dy;

		InsertRPnt(x2, y2);
		x1 = x2;
		y1 = y2;
	}
	{
		TYPE x2 = rcap->x2;
		TYPE y2 = rcap->y2;
		InsertRPnt(x2, y2);
	}
}

template <class TYPE>
void TLineEX<TYPE>::LineTranslate(TYPE *LineC, TYPE *LineL, TYPE *LineR, TYPE Dist)
{
	TYPE x1 = LineC[0];
	TYPE y1 = LineC[1];
	TYPE x2 = LineC[2];
	TYPE y2 = LineC[3];
	TYPE dx = x2 - x1;
	TYPE dy = y2 - y1;
	TYPE dl = (TYPE)sqrt(dx * dx + dy * dy);

	if (dl == (TYPE)0)
		return;
	dy = Dist * dy / dl;
	dx = Dist * dx / dl;

	LineL[0] = x1 - dy;
	LineL[1] = y1 + dx;
	LineL[2] = x2 - dy;
	LineL[3] = y2 + dx;

	LineR[0] = x1 + dy;
	LineR[1] = y1 - dx;
	LineR[2] = x2 + dy;
	LineR[3] = y2 - dx;
}

template <class TYPE>
TYPE TLineEX<TYPE>::TurningOfLine(TYPE *LineT)
{
	TYPE dX1 = LineT[2] - LineT[0];
	TYPE dY1 = LineT[3] - LineT[1];
	TYPE dX2 = LineT[4] - LineT[2];
	TYPE dY2 = LineT[5] - LineT[3];

	return dX1 * dY2 - dX2 * dY1;
}

template <class TYPE>
long TLineEX<TYPE>::LineIntersect(TYPE *LineA, TYPE *LineB, TYPE *Point)
{
	// A1 * X  +  B1 * Y  +  C1  =  0
	TYPE A1 = LineA[3] - LineA[1];
	TYPE B1 = LineA[0] - LineA[2];
	TYPE C1 = LineA[2] * LineA[1] - LineA[0] * LineA[3];

	TYPE L1 = A1 * LineB[0] + B1 * LineB[1] + C1;
	TYPE L2 = A1 * LineB[2] + B1 * LineB[3] + C1;

//	if (L1 != 0 && L2 != 0 && L1 * L2 > 0)
	if (L1 * L2 > 0)
		return DONT_INTERSECT;

	// A2 * X  +  B2 * Y  +  C2  =  0
	TYPE A2 = LineB[3] - LineB[1];
	TYPE B2 = LineB[0] - LineB[2];
	TYPE C2 = LineB[2] * LineB[1] - LineB[0] * LineB[3];

	L1 = A2 * LineA[0] + B2 * LineA[1] + C2;
	L2 = A2 * LineA[2] + B2 * LineA[3] + C2;

//	if (L1 != 0 && L2 != 0 && L1 * L2 > 0)
	if (L1 * L2 > 0)
		return DONT_INTERSECT;

	L1 = A1 * B2 - A2 * B1;
	if (L1 == 0)
		return LINE_COLLINEAR;

	Point[0] = (B1 * C2 - B2 * C1) / L1;
	Point[1] = (A2 * C1 - A1 * C2) / L1;

	return LINE_INTERSECT;
}


template <class TYPE>
class TSimpDP
{
public:
	static long SimpDP(TYPE *dst, TYPE *src, long  len, TYPE tol, char *wrk);
	static long SelfIT(TYPE *dst, long  len, char *wrk, long cnt);

protected:
	static long SimpIN(TYPE *dst, TYPE *sfr, TYPE *sto, TYPE tol, char *wrk);
	static void SimpIN(           TYPE *dfr, TYPE *dto, TYPE Tol, char *wrk);
	static long Retain(TYPE *dst, long  len, char *wrk);
	static TYPE DistPT(TYPE *src, TYPE *dst);
	static TYPE DistVL(TYPE *src, TYPE *pnt);

	enum
	{
		DONT_INTERSECT = 0,
		LINE_INTERSECT = 1,
		LINE_COLLINEAR = 2
	};
	static long LineIT(TYPE *lna, TYPE *lnb, TYPE *pnt);
};

template <class TYPE>
long TSimpDP<TYPE>::SimpDP(TYPE *dst, TYPE *src, long len, TYPE tol, char *wrk)
{
	if (len < 2)
		return len;

	dst[0] = src[0];
	dst[1] = src[1];

	TYPE tss = tol * tol;

	long end = len * 2 - 2;
	long dfr = 0;
	long sfr = 0;

	{
		dfr += SimpIN(dst + dfr, src + sfr, src + end, tss, wrk);
	}

	return SelfIT(dst, (dfr + 2) / 2, wrk, 4);
}

template <class TYPE>
long TSimpDP<TYPE>::SelfIT(TYPE *dst, long  len, char *wrk, long cnt)
{
	long run;

	len = len * 2 - 2;
	do
	{
		run = 0;
		memset(wrk, 0xFF, len);

		TYPE *dfr = dst;
		TYPE *dto = dst + len;

		for(TYPE *tmp = dfr + 0; tmp < dto; tmp += 2)
		{
			long num = 0;
			for(TYPE *cmp = tmp + 4; cmp < dto && num++ < cnt; cmp += 2)
			{
				TYPE pnt[2];
				if (LineIT(tmp, cmp, pnt) == LINE_INTERSECT)
				{
					for(TYPE *cur = tmp + 2; cur < cmp; cur += 2)
					{
						wrk[(cur - dfr) / 2] = 0;
					}

					cmp[0] = pnt[0];
					cmp[1] = pnt[1];

					tmp = cmp;
					run = -1;
					break;
				}
			}
		}

		if (run)
			len = Retain(dfr, dto - dfr, wrk);
	}
	while(run);

	return (len + 2) / 2;
}

template <class TYPE>
long TSimpDP<TYPE>::SimpIN(TYPE *dst, TYPE *sfr, TYPE *sto, TYPE tol, char *wrk)
{
	TYPE *frm = sfr + 2;
	TYPE *tmp = dst + 2;
	for(; frm < sto; frm += 2)
	{
		if (DistPT(tmp - 2, frm) > tol)
		{
			tmp[0] = frm[0];
			tmp[1] = frm[1];
			tmp += 2;
		}
	}
	if (tmp > dst + 2)
	{
		if (DistPT(tmp - 2, frm) < tol)
		{
			tmp -= 2;
		}
	}

	tmp[0] = frm[0];
	tmp[1] = frm[1];

	long len = tmp - dst;
	if (len <= 2 || wrk == NULL)
		return len;

	memset(wrk, 0x0, len / 2);
	SimpIN(dst, tmp, tol / 4, wrk);

	return Retain(dst, len, wrk);
}

template <class TYPE>
void TSimpDP<TYPE>::SimpIN(           TYPE *dfr, TYPE *dto, TYPE tol, char *wrk)
{
	if (dto - dfr < 4)
		return;

	TYPE max = 0;
	TYPE*bst = dfr + 2;

	{
		TYPE seg[4] = { dfr[0], dfr[1], dto[0], dto[1] };

		for(TYPE *tmp = bst; tmp < dto; tmp += 2)
		{
			TYPE len = DistVL(seg, tmp);
			if (max < len)
			{
				max = len;
				bst = tmp;
			}
		}
	}

	if (max > tol)
	{
		long idx = (bst - dfr) / 2;
		wrk[idx] = -1;

		SimpIN(dfr, bst, tol, wrk);
		SimpIN(bst, dto, tol, wrk + idx);
	}
}

template <class TYPE>
long TSimpDP<TYPE>::Retain(TYPE *dst, long  len, char *wrk)
{
	long idn = 2;
	long idx = 2;
	for(; idx < len; idx += 2)
	{
		if (wrk[idx / 2])
		{
			if (idx != idn)
			{
				dst[idn + 0] = dst[idx + 0];
				dst[idn + 1] = dst[idx + 1];
			}
			idn += 2;
		}
	}

	if (idx != idn)
	{
		dst[idn + 0] = dst[idx + 0];
		dst[idn + 1] = dst[idx + 1];
	}
	return idn;
}

template <class TYPE>
TYPE TSimpDP<TYPE>::DistPT(TYPE *src, TYPE *dst)
{
	TYPE dx = src[0] - dst[0];
	TYPE dy = src[1] - dst[1];
	return dx * dx + dy * dy;
}

template <class TYPE>
TYPE TSimpDP<TYPE>::DistVL(TYPE *src, TYPE *pnt)
{
	TYPE X1 = src[0];
	TYPE Y1 = src[1];
	TYPE X2 = src[2];
	TYPE Y2 = src[3];
	TYPE X0 = pnt[0];
	TYPE Y0 = pnt[1];

	TYPE dX = X2 - X1;
	TYPE dY = Y2 - Y1;

	TYPE dR = (Y0 - Y1) * dY - (X1 - X0) * dX;
	TYPE dL;
	if (dR <= (TYPE)0)
	{
		dX = X1;
		dY = Y1;
	}
	else if (dR >= (dL = dX * dX + dY * dY))
	{
		dX = X2;
		dY = Y2;
	}
	else
	{
		dX = X1 + dX * dR / dL;
		dY = Y1 + dY * dR / dL;
	}

	dX -= X0;
	dY -= Y0;
	return dX * dX + dY * dY;
}

template <class TYPE>
long TSimpDP<TYPE>::LineIT(TYPE *lna, TYPE *lnb, TYPE *pnt)
{
	// A1 * X  +  B1 * Y  +  C1  =  0
	TYPE A1 = lna[3] - lna[1];
	TYPE B1 = lna[0] - lna[2];
	TYPE C1 = lna[2] * lna[1] - lna[0] * lna[3];

	TYPE L1 = A1 * lnb[0] + B1 * lnb[1] + C1;
	TYPE L2 = A1 * lnb[2] + B1 * lnb[3] + C1;

//	if (L1 != 0 && L2 != 0 && L1 * L2 > 0)
	if (L1 * L2 > 0)
		return DONT_INTERSECT;

	// A2 * X  +  B2 * Y  +  C2  =  0
	TYPE A2 = lnb[3] - lnb[1];
	TYPE B2 = lnb[0] - lnb[2];
	TYPE C2 = lnb[2] * lnb[1] - lnb[0] * lnb[3];

	L1 = A2 * lna[0] + B2 * lna[1] + C2;
	L2 = A2 * lna[2] + B2 * lna[3] + C2;

//	if (L1 != 0 && L2 != 0 && L1 * L2 > 0)
	if (L1 * L2 > 0)
		return DONT_INTERSECT;

	L1 = A1 * B2 - A2 * B1;
	if (L1 == 0)
		return LINE_COLLINEAR;

	pnt[0] = (B1 * C2 - B2 * C1) / L1;
	pnt[1] = (A2 * C1 - A1 * C2) / L1;

	return LINE_INTERSECT;
}

typedef double REAL;

static int SimDetailPro(double* pDes, double* pSrc, int size, int levl, REAL toll) //以xyxy形式，pSrc 存储的点集合，size 是point array的大小
{
	REAL *coor = new REAL[1024 * 64 * 2];
	char *work = new char[1024 * 64];
	typedef TSimpDP<REAL> CSimp;

	long lens = CSimp::SimpDP(coor, (REAL *)pSrc, size, toll, work);
	memcpy(pDes, coor, sizeof(double) * lens * 2);

	delete[] coor;
	delete[] work;

	return lens;
}

static int SimLcPro(double* pDes, double* pSrc, int size, int levl) //以xyxy形式，pSrc 存储的点集合，size 是point array的大小
{	
	REAL tol_set = 1.2;
	REAL toll = 360.0 * tol_set / (REAL)(1 << (levl + 8));

	return SimDetailPro(pDes, pSrc, size, levl, toll);
}

static int SimLinePro(double* pDes, double* pSrc, int size, int levl) //以xyxy形式，pSrc 存储的点集合，size 是point array的大小
{	
	REAL tol_set = 0.5; 
	REAL toll = 360.0 * tol_set / (REAL)(1 << (levl + 8));

	return SimDetailPro(pDes, pSrc, size, levl, toll);
}

static int SimLineProBySmall(double* pDes, double* pSrc, int size, int levl) //以xyxy形式，pSrc 存储的点集合，size 是point array的大小
{	
	REAL tol_set = 1.5;
	REAL toll = 360.0 * tol_set / (REAL)(1 << (levl + 8));

	return SimDetailPro(pDes, pSrc, size, levl, toll / 16.0);
}

static void DivdeDL(double* pLDes, double* pRDes, double* pSrc, int size, int& LDesLen, int& RDesLen, int levl) // 单线双向路分离
{
	REAL tol_set = 1.5;
	REAL toll = 360.0 * tol_set / (REAL)(1 << (levl + 8));
	
	REAL offs = 4e-5;
	REAL *coor = new REAL[1024 * 64 * 2];
	char *work = new char[1024 * 64];

	typedef TSimpDP<REAL> CSimp;
	typedef TLineEX<REAL> CLine;
	CLine line;
	line.SetLnWidth(offs);
	line.SetLineCap(SQUARE_CAP);
	line.SetJoinCap(ROUNDS_CAP);
	
	line.MoveTo(pSrc[0], pSrc[1]);
	for(long idxs = 2; idxs < size * 2; )
	{
		REAL x = pSrc[idxs++];
		REAL y = pSrc[idxs++];
		line.LineTo(x, y);
	}
	line.Finish();

	// 分离
    line.SelfIT(work, SelfITDist[levl]);
//	line.m_SizeL = CSimp::SelfIT(line.m_BuffL, line.m_SizeL, work, 16);
	int lens = CSimp::SimpDP(coor, (REAL *)line.m_BuffL, line.m_SizeL, toll / 16.0, work);
	memcpy(pLDes, coor, sizeof(double) * lens * 2);
	LDesLen = lens;

//	line.m_SizeR = CSimp::SelfIT(line.m_BuffR, line.m_SizeR, work, 16);
	lens = CSimp::SimpDP(coor, (REAL *)line.m_BuffR, line.m_SizeR, toll / 16.0, work);
	memcpy(pRDes, coor, sizeof(double) * lens * 2);
	RDesLen = lens;

	delete[] coor;
	delete[] work;
}

//平移道路 //以xyxy形式，pSrc 存储的点集合，size 是point array的大小
static int MoveLine(vector<double>& vtDes, vector<double>& vtSrc, bool IsLeft, int levl) 
{	
	int size  = vtSrc.size() / 2; // pnt size
	
	REAL off_set = (levl == 12) ? 6 :9;
	//REAL off_set = (levl == 12) ? 6 : 12; // road shape is 3, name is 8
	REAL offs = 360.0 * off_set / (REAL)(1 << (levl + 8));

	REAL tol_set = 1;
	REAL toll = 360.0 * tol_set / (REAL)(1 << (levl + 8));

	REAL *coor = new REAL[1024 * 64 * 2];
	char *work = new char[1024 * 64];
	long lens = size;

	typedef TSimpDP<REAL> CSimp;
	typedef TLineEX<REAL> CLine;
	CLine line;
	line.SetLnWidth(offs);
	line.SetLineCap(SQUARE_CAP);
	line.SetJoinCap(ROUNDS_CAP);
	
	line.MoveTo(vtSrc[0], CVEProjection::l2m( vtSrc[1] ) );
	for(long idxs = 2; idxs < lens * 2; )
	{
		REAL x = vtSrc[idxs++];
		REAL y = CVEProjection::l2m(vtSrc[idxs++]);
		line.LineTo(x, y);
	}
	line.Finish();

	if((line.m_SizeL == 0)
		||(line.m_SizeR == 0))
	{
		delete[] coor;
		delete[] work;
		return 0;
	}

	// 偏移
    line.SelfIT(work, SelfITDist[levl]);
	if(IsLeft)
	{
		lens = CSimp::SimpDP(coor, (REAL *)line.m_BuffL, line.m_SizeL, toll, work);
	}
	else
	{
		lens = CSimp::SimpDP(coor, (REAL *)line.m_BuffR, line.m_SizeR, toll, work);
	}

	vtDes.clear();
	for(size_t i = 0; i < lens; ++i)
	{
		vtDes.push_back(coor[2 * i]);
		vtDes.push_back(CVEProjection::m2l( coor[2 * i + 1] ) );
	}
	delete[] coor;
	delete[] work;
	return lens;
}

static int SelfIT(double *dst, long  len, long cnt)
{
    char* work = new char[1024 * 64];
    int iSize = 0;
    if(!work)
    {
        return iSize;
    }
    iSize = TSimpDP<double>::SelfIT(dst, len, work, cnt);
    delete [] work;
    return iSize;
}

#endif