#include "CGetDTILink.h"

// 操作类型， 1是输出dti对应的道路名称
//            2是输出所有道路名称
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
	cout<<"操作类型， 1是输出dti对应的道路名称\n";
	cout<<"           2是输出所有道路名称\n";

	cin>>nOperator;
	LoopFiles();
	cout<<"over\n";

	system("puase");
}