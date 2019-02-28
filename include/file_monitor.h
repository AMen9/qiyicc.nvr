#ifndef FILEMONITOR_H
#define FILEMONITOR_H

#include<map>
#include<vector>
#include<iostream>

using namespace std;

enum EventType
{
	CREATE,
	DELETE,
	MODIFY
};

typedef struct 
{
    string 		path;		//·��
    EventType 	type;		//�¼�����
    int 		fileType;	//0��ʾ�ļ���,1��ʾ�ļ�
} Event ;

typedef struct 
{
    string 		path;		//�ļ�·��
    int 		expire;		//����ʱ��
} FileInfo;

typedef void (*ListenerProc)(const Event *)	;

/////////////////////////////////////////////////////////////////
//
// class CFileMonitor
//
/////////////////////////////////////////////////////////////////
class CFileMonitor
{
public:
	CFileMonitor(std::string path, ListenerProc proc, int expire=180);
	virtual ~CFileMonitor();

	int start();

protected:
	void watchFolder(const std::string & path);
	void deleteFile();

private:
	int m_fd ;						//���
	long m_expire ;					//�ļ�����ʱ��
	std::string m_path ;				//��ظ�Ŀ¼	
	std::map<int,string> monitorMap;		//����б�	
	std::vector<FileInfo *> fileList;
	ListenerProc listener ;
};

#endif // FILEMONITOR_H
