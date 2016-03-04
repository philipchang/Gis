/**
 * @des: ��ȡR���DTIӳ�������meshlink��linkInfo��ӳ��
 *
 */

#ifndef _CGET_DTI_LINK_H
#define _CGET_DTI_LINK_H
#pragma once

#include "ComDef.h"
#include "CRLDataReader.h"

class CGetDTILink
{
public:
	CGetDTILink();
	~CGetDTILink();

	/**
	 * @brief; ��ʼ��,��ȡtab��ʽ�ĵ�ͼ�ļ�
	 * @input  nOperator,�������ͣ�
	 *			1�����dti��Ӧ�ĵ�·����
	 *          2��������е�·����
	 */
	bool Process(string& strCityDir, int nOperator );		
private:
	void ReadDti();
	void PushOneDtiInfo(string& strDtiInfo);
	void PushRoadName();
	void Output(string& strOutputCity);
	void LoopFiles(string strCityDir);
	
private:
	size_t					m_nADCode;
	string					m_strR_LNameDir;
	string					m_strR_NameDir;
	string					m_strDITCsvDir;

	map<string, CLinkInfo>	m_mapLinkID2Info;//<meshlink, stLinkInfor>

	CRLDataReader			m_RLReader;
};


#endif