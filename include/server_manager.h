#ifndef _SERVERMANAGER_H
#define _SERVERMANAGER_H

// ���ε��������
#ifdef WIN32
#pragma warning (disable:4786)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <json/json.h>
#include "jmutex.h"
#include "thrift_server.h"
#include "public_common.h"

// ********************************************************
//
// class CServerManager �ඨ��ʵ��
//
// ********************************************************
class CServerManager
{
public:
	CServerManager();
	virtual ~CServerManager();

	int inital();
	int unInit();
	bool isRun();

	// ��ӡ�������Ϣ
	void dump();
	// ִ�еȴ�����
	void wait();	
	// ��������
	void demo();
	// װ������
	void loadConfig();
	// д��־
	void writeLog(const std::string & message);
	
	// ���ز���
	int loadPolicy();
	// ���ع���
	int loadCabinet();
	// ��������ͷ
	int loadCamera();
	// ���ز���
	int downloadPolicy();
	// ���ع����б�
	int downloadCabinets();
	// ��������ͷ�б�
	int downloadCameras();
	// ���ػ���
	int loadCache(const std::string & fileName, Json::Value & json);
	// ���ݱ���
	int saveCache(const std::string & fileName, Json::Value & json);
	// ���ع���
	int downloadCommon(const std::string & url, Json::Value & json);
	// ����policy
	int processPolicy(Json::Value & json);
	// ��������
	int processCabinet(Json::Value & json);
	// ��������ͷ
	int processCamera(Json::Value & json);

	// ����������Ϣ
	void reloadConfig();

	// ¼�������˳�
	int taskExit(pid_t pid, int status);

	// ��ȡ����·��
	std::string getCachePath();
	// ��ȡ�ϴ�·��
	std::string getUploadPath();
	// ��ȡ¼��·��
	std::string getRecorderPath();
	// ��ȡ�ӽ�����־
	std::string getChildLogPath(pid_t pid);
	// ��ȡ����ͷ��ַ
	std::string getCameraPath(int id);
	// ��ȡ����ͷ¼��
	std::string getCameraFile(int id, const std::string & format);
	// ��ȡ��ǰ����
	std::string getCurrentTime();
	// ��ȡ��ǰ�����ܼ�
	TASKTIME getTaskTime();	

public:

	// �ɷ�����
	int dispatchTask();	
	// ����һ��¼������
	int createTask(int id);
	// ִ��һ������
	int startTask(TASKOBJECT & taskObject);
	// ֹͣ����
	int stopTask(TASKOBJECT & taskObject);
	// ���չ����ļ�
	int recycleExpired();


protected:
	bool						m_bInit ;
	bool						m_bDemo ;
	// �����ļ���·��
	std::	string				m_workPath ;
	std::string				m_logPath ;
	std::string				m_childPath ;

	// �����������ַ
	std::string				m_token ;
	std::string				m_webServer ;
	int						m_expired ;
	bool						m_bExpired ;
	TIMESTATE				m_timeExpired ;
	CThriftServer				m_thriftServer ;

	JMutex					m_mutexPolicy ;
	std::string				m_policyVersion ;
	QuePolicyObject			m_quePolicy ;

	JMutex					m_mutexCabinets ;
	std::string				m_cabinetVersion ;
	HashCabinetObject			m_hashCabinets ;

	JMutex					m_mutexCameras ;
	std::string				m_cameraVersion ;
	HashCameraObject			m_hashCameras ;

	JMutex					m_mutexTask ;
	HashTaskObject			m_hashTask ;
};

extern CServerManager		ServerManager ;

#endif
