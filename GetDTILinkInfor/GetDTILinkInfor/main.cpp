#include "CGetDTILink.h"

// �������ͣ� 1�����dti��Ӧ�ĵ�·����
//            2��������е�·����
int nOperator = 1;
void Process(string& strCityDir)
{
	CGetDTILink test;	
	test.Process(strCityDir, nOperator);
}

void LoopFiles( )
{	
	string strDataDir("./Data/");
	char DirFind[256] = {0};
	sprintf_s(DirFind,"%s/*.*", strDataDir.c_str());
	struct _finddata_t FileInfo;
	long FileHandle = _findfirst(DirFind, &FileInfo);
	
	static int iCity = 0;
	if(FileHandle != -1)
	{
		do{
			int fileNameLen = strlen(FileInfo.name);
			if( fileNameLen > 2 )
			{
				string strcityDir(strDataDir);
				strcityDir += FileInfo.name;
				cout<<++iCity<<"	index  "<<strcityDir<<endl;
				Process(strcityDir);					
			}

		}while(_findnext(FileHandle, &FileInfo) == 0);
		_findclose(FileHandle);
	}
}


int main(void)
{
	cout<<"�������ͣ� 1�����dti��Ӧ�ĵ�·����\n";
	cout<<"           2��������е�·����\n";

	cin>>nOperator;
	LoopFiles();
	cout<<"over\n";

	system("puase");
}