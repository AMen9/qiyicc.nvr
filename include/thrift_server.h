#ifndef _THRIFT_SERVER_H
#define _THRIFT_SERVER_H

// ���ε��������
#ifdef WIN32
#pragma warning (disable:4786)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nvrWebService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thread>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::com::nvr::thrift;

namespace thrift = ::apache::thrift::stdcxx ;

#define nvrShared thrift::shared_ptr

// ********************************************************
//
// class CServerManager �ඨ��ʵ��
//
// ********************************************************
class CThriftServer
{
public:
	CThriftServer();
	virtual ~CThriftServer();

	// ����thrift  ����
	int start();
	// ֹͣthrift ����
	int stop();

	// ���ö˿�
	void setPort(int port);
	// �����߳���
	void setThread(int count);
	// ���÷��ͳ�ʱ
	void setSendTimeout(int timeout);
	// ���ý��ճ�ʱ
	void setRecvTimeout(int timeout);
	

protected:
	// �߳�����
	void run();

protected:
	int								mPort ;
	int								mCount ;
	int								mSendTimeout ;
	int								mRecvTimeout ;		
	std::thread						mThread ;	
	nvrShared<TThreadPoolServer> 		mServer ;
};

#endif //

