#ifndef __PUBLIC_COMMON_20181118074800_H
#define __PUBLIC_COMMON_20181118074800_H

// ���ε��������
#ifdef WIN32
#pragma warning (disable:4786)
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dirent.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sstream>
#include <iostream>  
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <list>
#include <map>
#include <string>
#include <deque>
#include <string.h>
#include <unordered_map>

using std::list ;
using std::map ;
using std::string ;
using std::deque ;

// ����windowns UNICODE ��ʽ����
#define _T(x)				x
#define TCHAR			char
#define tstrlen			strlen
#define tstrcpy			strcpy
#define tstrncpy			strncpy
#define tstrcmp			strcmp
#define tstrncmp     		strncmp
#define tstrcat                 	strcat
#define tstrstr         		strstr
#define tprintf			printf
#define tfprintf			fprintf
#define tsprintf			sprintf
#define tstring 			std::string

typedef unsigned int		DWORD ;

typedef enum WeekDay
{
	// δ����
	wdNone			=	-1,
	// ����
	wdSun			=	0,
	// ��һ
	wdMonday 		=	1,
	// �ܶ�
	wdTuesday 		=	2,
	// ����
	wdWednesday 	=	3,
	// ����
	wdThursday 		=	4,
	// ����
	wdFriday 		=	5,
	// ����
	wdSaturday 		=	6
}WEEKDAY ;

typedef struct _tagDayPoint
{
	// ��
	int				year ;
	// ��
	int				month ;
	// ��
	int				day ;
}DAYPOINT, *LPDAYPOINT ;

typedef struct _tagTimePoint
{
	// ʱ
	int				hour ;
	// ��
	int				minute ;
	// ��
	int				seconds ;

	_tagTimePoint() : hour(0), minute(0), seconds(0)
	{}

	// ��ȡֵ
	int getValue()
	{
		return hour * 3600 + minute * 60 + seconds ;
	}		
}TIMEPOINT, *LPTIMEPOINT ;

typedef struct _tagTaskTime
{
	// ���ڵ�
	DAYPOINT	dayPoint ;
	// ʱ���
	TIMEPOINT	timePoint ;
	// �ܼ�
	WEEKDAY	weekDay ;	
}TASKTIME, *LPTASKTIME ;

typedef struct _tagExpiredPolicy
{
	// ��ʱ����
	int			expired ;
	// ·��
	std::string	path ;
}EXPIREDPOLICY, *LPEXPIREDPOLICY ;

typedef struct _tagAlarmParam
{
	// IM ֪ͨ
	std::string		im ;
	// ����֪ͨ
	std::string		sms ;
	// �ʼ�֪ͨ
	std::string		email ;
}ALARMPARAM, *LPALARMPARAM ;

typedef struct _tagCameraParam
{
	// ��Ƶ��С
	std::string		videoSize ;
	// ��Ƶ����
	std::string		videoSample ;
	// ��Ƶ���� h264
	std::string		videoEncoder ;
	// ��Ƶ����
	std::string		audioSample ;
	// ��Ƶ����aac
	std::string		audioEncoder ;
}CAMERAPARAM, *LPCAMERAPARAM ;

typedef std::deque<int>	QueCameras ;

typedef struct _tagCabinetObject
{
	// NVR ID
	int				pid ;
	// ����ID
	int				id ;
	// ��������
	std::string		name ;
	// ����γ��
	double			latitude ;
	// ���徭��
	double			longitude ;
	// ���������ͷ�б�
	QueCameras		cameras ;
}CABINETOBJECT, *LPCABINETOBJECT ;

typedef std::deque<std::string>		QueProtocols ;

typedef struct _tagCameraObject
{
	// ����ID
	int				pid ;
	// ����ͷID
	int				id ;
	// ���Ա��
	int				policy ;
	// ����ͷ����
	std::string		name ;
	// ����ͷ����
	std::string		type ;
	// ��֤��
	std::string		authUser ;
	// ��֤����
	std::string		authPassword ;
	// ��������
	std::string		osd ;
	// ֡������
	std::string		fps ;
	// Ĭ��¼��Э������
	unsigned int		protocolIdx ;
	// ֧�ֵ�дЭ���б�"onvif", "rtmp", "http"
	QueProtocols		protocols ;
}CAMERAOBJECT, *LPCAMERAOBJECT ;

typedef struct _tagTimeSpan
{
	// ��ʼ¼��ʱ��
	TIMEPOINT		from ;
	// ����¼��ʱ��
	TIMEPOINT		to ;
}TIMESPAN, *LPTIMESPAN ;

typedef std::deque<TIMESPAN>	QueTimeSpan;

typedef struct _tagPolicyObject
{
	// ����ID
	int				id ;
	// ��������
	std::string		name ;
	// �洢�ռ� 0  ��ʾ������
	int				storage ;
	// �洢�ļ�����ʱ��0  ��ʾ��������
	int				expired ;
	// ¼���ļ�ʱ��
	int				segment ;
	// ¼��Ԥ��ͼƬ��С 320x240
	std::string		preview ;
	// ¼���ļ���ʽmp4, flv, hls
	std::string		format ;
	// ¼���������
	CAMERAPARAM	camera ;
	// ������������
	ALARMPARAM		alarm ;
	// ¼��ʱ���
	QueTimeSpan		weeks[7] ;
}POLICYOBJECT, *LPPOLICYOBJECT ;

typedef struct _tagTaskParam
{
	// ¼�Ƶ�URL
	std::string		url ;	
	// ¼��ʱ��
	std::string		time ;
	// ¼�Ƶĸ�ʽ
	std::string		format ;
	// ������ļ���ʽ
	std::string 		path ;
}TASKPARAM, *LPTASKPARAM ;

typedef enum TaskState
{
	// ��ʼ��
	stateInit	=	0,
	// ��ʼ
	stateBegin	=	1,
	// ����
	stateIdle	=	2,
	// ��ʼ
	stateStart	=	3,
	// ��ͣ
	statePause	=	4,
	// ����
	stateRun	=	5,
	// ֹͣ
	stateStop	=	6,
	// ����
	stateEnd	=	7,
}TASKSTATE;

typedef struct _tagTaskObject
{
	// ����ͷ���
	int				camera ;
	// ����ID
	int				policy ;
	// ��Ӧ�Ľ���
	pid_t			child ;
	// �������
	TASKPARAM		param ;
	// ����ʱ���
	QueTimeSpan		weeks[7] ;
}TASKOBJECT, *LPTASKOBJECT ;

// �ļ���С
typedef struct _tagFileInfo
{
	tstring		sName ;							// �ļ�����
	DWORD		nFileSizeHigh; 					// �ļ���С
	DWORD		nFileSizeLow;					// �ļ���С
	struct tm 	tmCreate ;						// �ļ�����
} FILEINFO, *LPFILEINFO ;

// ��������δ���û���ʱ�ṹ����
typedef struct _tagTimeState
{
#ifdef WIN32
	DWORD				tmLogon ;					// ��¼ʱ��
#else
	struct timeval		tmLogon ;					// ��¼ʱ��
#endif	

	_tagTimeState(bool bTime=false)
	{
		if ( bTime )
		{
			ResetTime () ;				
		}
		else
		{
#ifdef WIN32
			tmLogon			=	0 ;
#else
			tmLogon .tv_sec	=	0 ;
			tmLogon .tv_usec	=	0 ;
#endif					
		}
	}

	_tagTimeState & operator=(const _tagTimeState & rhs)
	{
		tmLogon	=	rhs .tmLogon ;					// ��¼ʱ��
		return *this ;
	}

	void ResetTime()
	{
#ifdef WIN32
		tmLogon		=	::GetTickCount () ;		
#else
		// clock ��AS4 (64) �Ļ����Ϸ���0����֪��ʲôԭ��
		::gettimeofday ( &tmLogon, NULL ) ;
#endif
	}

	void ResetTime(int nRemainTime)
	{
#ifdef WIN32
		tmLogon			=	::GetTickCount () - nRemainTime ;		
#else
		// clock ��AS4 (64) �Ļ����Ϸ���0����֪��ʲôԭ��
		::gettimeofday ( &tmLogon, NULL ) ;
		nRemainTime	=	tmLogon .tv_sec * 1000 + tmLogon .tv_usec / 1000 - nRemainTime ;
		tmLogon .tv_sec	=	nRemainTime / 1000 ;
		tmLogon .tv_usec	=	(nRemainTime - tmLogon .tv_sec * 1000) * 1000 ;
#endif

	}

	// �Ƿ�ʱ
	bool IsTimeOut(int nTimeOut)
	{
#ifdef WIN32
		DWORD	tmCurrent 	=	::GetTickCount () ;
		int		duration 	=	tmCurrent - tmLogon ;
#else
		struct timeval	tmCurrent ;
		::gettimeofday ( &tmCurrent, NULL ) ;
		
		int		duration 	=	( tmCurrent .tv_sec - tmLogon .tv_sec ) * 1000 + ( tmCurrent .tv_usec - tmLogon .tv_usec )  / 1000 ;
#endif

		return (duration >= nTimeOut) ? true : false ;
	}

	// �Ƿ�ʱ
	bool IsTimeOut(_tagTimeState & time, int nTimeOut)
	{
#ifdef WIN32
		int	duration = time .tmLogon - tmLogon ;
#else
		
		int	duration = ( time .tmLogon .tv_sec - tmLogon .tv_sec ) * 1000 + ( time .tmLogon .tv_usec - tmLogon .tv_usec )  / 1000 ;
#endif

		return (duration >= nTimeOut) ? true : false ;
	}

	// ��ȡ��ʾ
	DWORD ToToken()
	{
#ifdef WIN32
		return tmLogon ;
#else
		return DWORD(( tmLogon .tv_sec ) * 1000 + ( tmLogon .tv_usec )  / 1000) ;
#endif
	}

	// ����ʱ��
	int Duration()
	{
#ifdef WIN32
		DWORD	tmCurrent 	=	::GetTickCount () ;
		// ת��Ϊ����
		int		duration 	=	tmCurrent - tmLogon ;
#else
		struct timeval	tmCurrent ;
		::gettimeofday ( &tmCurrent, NULL ) ;

		// ת��Ϊ����
		int		duration 	=	( tmCurrent .tv_sec - tmLogon .tv_sec ) * 1000 + ( tmCurrent .tv_usec - tmLogon .tv_usec )  / 1000 ;
#endif	
		return duration ;
	}
}TIMESTATE, *LPTIMESTATE ;

typedef std::deque<pid_t>								QueExitTask ;
typedef std::deque<POLICYOBJECT>						QuePolicyObject ;
typedef std::unordered_map<int, CABINETOBJECT>		HashCabinetObject ;
typedef std::unordered_map<int, CAMERAOBJECT>			HashCameraObject ;
typedef std::unordered_map<int, TASKOBJECT>			HashTaskObject ;

static std::string int_str(int id)
{
	std::ostringstream os ;
	os << id ;
	return os .str () ;
}

static int safe_strcpy(TCHAR * strDest, const TCHAR * strSource, size_t count)
{
        size_t length = tstrlen ( strSource ) ;

        if ( length == 0 )
        {
                return 0 ;
        }

        if ( length < count )
        {
                count = length ;
        }
        tstrncpy ( strDest, strSource, count ) ;
        strDest[count] = 0 ;
        return 0 ;
}

static void string_split(const std::string & input, const std::string & find, std::vector<std::string> & vValue)
{
	std::string         strOut ( input ) ;
	
       while ( true )
	{
		size_t pos = strOut .find ( find ) ;
		if ( pos == string::npos )
		{
			if ( false == strOut .empty () )
			{
				vValue .push_back ( strOut ) ;
			}
			break ;
		}

		std::string sHostIP = strOut .substr ( 0, pos ) ;
		strOut .erase ( 0, pos + find .size () ) ;
		vValue .push_back ( sHostIP ) ;
	}
}

static bool createDirectory(const TCHAR * szPath)
{
        if ( szPath == NULL || tstrlen ( szPath ) <= 0 )
        {
                return false ;
        }

#ifdef WIN32

        tstring sPath = ToplevelStudio::string_replace ( szPath, _T("/"), _T("\\") ) ;

        WIN32_FIND_DATA         wfd ;
        HANDLE                          hFind = FindFirstFile ( sPath .c_str (), &wfd ) ;
        if ( (hFind != INVALID_HANDLE_VALUE) && (wfd .dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
        {
                FindClose ( hFind ) ;
                return true ;
        }
        FindClose ( hFind ) ;

        if ( _T('\\') != (*sPath .rbegin ()) )
        {
                sPath += _T('\\') ;
        }

        bool bSuccess = false;
        tstring strTemp;
        std::vector<tstring> vPath;

        for ( uint32_t i = 0 ; i < sPath .size () ; ++i )
        {
                if ( sPath .at ( i ) != _T('\\') )
                {
                        strTemp += sPath .at ( i );
                }
                else
                {
                        vPath .push_back ( strTemp ) ;
                        strTemp += _T('\\') ;
                }
        }

        std::vector<tstring>::const_iterator vIter ;
        for ( vIter = vPath .begin () ; vIter != vPath .end () ; vIter++ )
        {
                if ( TRUE == ::CreateDirectory ( (*vIter) .c_str (), NULL ) || 183 == ::GetLastError () )
                {
                        bSuccess = true ;
                }
                else
                {
                        bSuccess = false ;
                        ToplevelStudio::Log .Error ( FILELINE, _T("createDirectory - %s failed ( %d )!"), sPath .c_str (), ::GetLastError () ) ;
                }
        }

        return bSuccess ;
		
#else

        char szNewPath [260] = {0} ;
        safe_strcpy ( szNewPath, szPath, 256 ) ;

        int len = ::strlen ( szNewPath ) ;
        if ( szNewPath [ len - 1] != '/' )
        {
                ::strcat ( szNewPath, "/" ) ;
        }

        len += 1 ;

        for ( int i = 1 ; i < len ; i++ )
        {
                if ( szNewPath [i] == '/' )
                {
                        szNewPath [i] = 0 ;
                        if ( ::access ( szNewPath, R_OK ) != 0 )
                        {
                                if ( ::mkdir ( szNewPath, 0755 ) == -1 )
                                {
                                        if ( errno == EEXIST )
                                        {
                                                continue ;
                                        }
                                        return false ;
                                }
                        }
                        szNewPath [i] = '/' ;
                }
        }
		
#endif

        return true ;
}

// �Ƿ��Ǹ�Ŀ¼
static bool isRootPath(const TCHAR * szPath)
{
	TCHAR		szRoot [32]	=	{0} ;
	tsprintf ( szRoot, _T("%c:\\"), szPath [0] ) ; 
	return ( tstrcmp ( szRoot, szPath ) == 0 ) ; 
}

// ����һ���ļ����е�����Ŀ¼
static int recursionDir(const TCHAR * pszPath, const TCHAR * pszHook, deque<tstring> & listDirName)
{
	if ( pszPath == NULL )
	{
		return -1 ;
	}

#ifdef WIN32
	TCHAR		szFind [ 258 ]	=	{0} ;

	safe_strcpy ( szFind, pszPath, tstrlen ( pszPath ) > 256 ? 256 : tstrlen ( pszPath ) ) ; 

		if ( !isRootPath ( szFind ) )
		{
			tstrcat ( szFind, _T("\\") ) ; 
		}

	tstrcat ( szFind, _T("*.*") ) ; // �������ļ� 

	WIN32_FIND_DATA 	wfd ;
	HANDLE 				hFind = ::FindFirstFile ( szFind, &wfd ) ;
	// ���û���ҵ������ʧ�� 
	if ( hFind == INVALID_HANDLE_VALUE ) 
	{
		return -2 ;
	}

	do
	{
		// ����������Ŀ¼ 
		if ( wfd .cFileName [0] == _T('.') ) 
		{
			continue ;
		}

		if ( (wfd .dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FALSE )
		{
			continue ;
		}

		if ( pszHook && tstrstr ( wfd .cFileName, pszHook ) )
		{
			listDirName .push_back ( tstring ( wfd .cFileName ) ) ;
		}
		else if ( NULL == pszHook )
		{
			listDirName .push_back ( tstring ( wfd .cFileName ) ) ;
		}
		
		// ���ļ����в��� 
	}while ( ::FindNextFile ( hFind, &wfd ) ) ;

	::FindClose ( hFind ) ;
#else
	struct stat 		info ;
	struct dirent *	dirent ;
	DIR * 			dir 				= 	NULL ;

	if ( ( ::lstat ( pszPath, &info ) ) != 0 ) 
	{
		return -2 ;
       }
		
	dir = ::opendir ( pszPath ) ;
	if ( dir == NULL )
	{
		return -3 ;
	}
	
	// ��ȡĿ¼�����������
	while ( ( dirent = ::readdir ( dir ) ) != 0 )
	{
		char		szFilename [ 512 ]	=	{0} ;		
		tsprintf ( szFilename, "%s/%s", pszPath, dirent->d_name ) ;
		
		if ( ::stat ( szFilename, &info ) != 0 ) 
		{
			continue ;
		}

		if ( ::strcmp ( dirent->d_name, "..") == 0 || ::strcmp ( dirent->d_name, ".") == 0 )
		{
			continue ;
		}

		//"<td> �ļ���С</td>"
		if (S_ISREG(info.st_mode))					{}
		// �����Ŀ¼���������ȡ��С
		else if ( S_ISDIR ( info.st_mode ) )			
		{
			if ( pszHook && ::strstr ( dirent->d_name, pszHook ) )
			{
				listDirName .push_back ( dirent->d_name ) ;
			}				
			else if ( NULL == pszHook )
			{
				listDirName .push_back ( dirent->d_name ) ;
			}
		}
		// "<td>����</td>"
		else if (S_ISLNK(info.st_mode))				{}
		// "<td>�ַ��豸</td>" 
		else if (S_ISCHR(info.st_mode))				{}
		// "<td>���豸</td>"
		else if (S_ISBLK(info.st_mode))				{}
		// "<td>FIFO</td>"
		else if (S_ISFIFO(info.st_mode))				{}
		// "<td>Socket</td>"
		else if (S_ISSOCK(info.st_mode))			{}
		// "<td>(δ֪)</td>"
		else										{}		

	}

	::closedir ( dir ) ;
#endif

	return 0 ;	

}


// ����һ���ļ����е������ļ�
static int recursionFile(const TCHAR * pszPath, const TCHAR * pszFilter, deque<FILEINFO> & listFileInfo)
{
	if ( pszPath == NULL )
	{
		return -1 ;
	}

#ifdef WIN32
	TCHAR	szFind [ 258 ]	=	{0} ;
	tstring	sPath = string_replace ( pszPath, _T("/"), _T("\\") ) ;

	safe_strcpy ( szFind, sPath .c_str (), sPath .size () > 256 ? 256 : sPath .size () ) ; 

		if ( !isRootPath ( szFind ) )
		{
			tstrcat ( szFind, _T("\\") ) ; 
		}

	tstrcat ( szFind, _T("*.*") ) ; // �������ļ� 

	WIN32_FIND_DATA 	wfd ;
	HANDLE 				hFind = ::FindFirstFile ( szFind, &wfd ) ;
	// ���û���ҵ������ʧ�� 
	if ( hFind == INVALID_HANDLE_VALUE ) 
	{
		return -2 ;
	}

	do
	{
		// ����������Ŀ¼ 
		if ( wfd .cFileName [0] == _T('.') ) 
		{
			continue ;
		}

		if ( wfd .dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			continue ;
		}

		if ( pszFilter && tstrstr ( wfd .cFileName, pszFilter ) )
		{
			continue ;
		}

		FILEINFO		fileInfo ;
		SYSTEMTIME	time ;
		
		::FileTimeToSystemTime ( &wfd .ftCreationTime, &time ) ;

		/*
		char		Date[512] = {0} ;

		::memset ( szDate, 0, 512 ) ;
		::sprintf ( szDate, "%04d-%02d-%02d %02d:%02d:%02d", time .wYear, 
			time .wMonth, time .wDay, time .wHour, time .wMinute, time .wSecond ) ;
		*/

		fileInfo .sName				=	tstring ( wfd .cFileName ) ;		
		fileInfo .nFileSizeLow			=	wfd .nFileSizeLow ;
		fileInfo .nFileSizeHigh			=	wfd .nFileSizeHigh ;

		fileInfo .tmCreate .tm_year		=	time .wYear - 1900 ;
		fileInfo .tmCreate .tm_mon		=	time .wMonth - 1 ;
		fileInfo .tmCreate .tm_mday		=	time .wDay ;
		fileInfo .tmCreate .tm_hour		=	time .wHour ;
		fileInfo .tmCreate .tm_min		=	time .wMinute ;
		fileInfo .tmCreate .tm_sec		=	time .wSecond ;		
		
		listFileInfo .push_back ( fileInfo ) ;
		// ���ļ����в��� 
	}while ( ::FindNextFile ( hFind, &wfd ) ) ;

	::FindClose ( hFind ) ;
#else
	struct stat 		info ;
	struct dirent *	dirent ;
	DIR * 			dir 			= 	NULL ;

	if ( ( ::lstat ( pszPath, &info ) ) != 0 ) 
	{
		return -2 ;
       }
		
	dir = ::opendir ( pszPath ) ;
	if ( dir == NULL )
	{
		return -3 ;
	}
	
	// ��ȡĿ¼�����������
	while ( ( dirent = ::readdir ( dir ) ) != 0 )
	{
		TCHAR		szFilename [ 512 ]	=	{0} ;		
		tsprintf ( szFilename, "%s/%s", pszPath, dirent->d_name ) ;
		
		if ( ::stat ( szFilename, &info ) != 0 ) 
		{
			continue ;
		}

		if ( ::strcmp ( dirent->d_name, "..") == 0 || ::strcmp ( dirent->d_name, ".") == 0 )
		{
			continue ;
		}

		//"<td> �ļ���С</td>"
		if ( S_ISREG ( info.st_mode ) )
		{
			if ( pszFilter && ::strstr ( dirent->d_name, pszFilter ) )
			{
				continue ;
			}

			FILEINFO		fileInfo ;
			struct tm *	time = ::localtime ( &info .st_mtime ) ;

			/*
			time->tm_year += 1900 ;
			time->tm_mon ++ ;
			sDate += outputString ( FILELINE, "%04d-%02d-%02d %02d:%02d:%02d", time->tm_year, 
				time->tm_mon, time->tm_mday, time ->tm_hour, time ->tm_min, time ->tm_sec ) ;
			*/
			
			fileInfo .sName		=	dirent->d_name ;
			fileInfo .tmCreate		=	* time ;
			fileInfo .nFileSizeLow	=	info .st_size ;				
			listFileInfo .push_back ( fileInfo ) ;
		}
		// �����Ŀ¼���������ȡ��С
		else if ( S_ISDIR ( info.st_mode ) )			{}
		// "<td>����</td>"
		else if ( S_ISLNK ( info.st_mode ) )			{}
		// "<td>�ַ��豸</td>" 
		else if ( S_ISCHR ( info.st_mode ) )			{}
		// "<td>���豸</td>"
		else if ( S_ISBLK ( info.st_mode ) )			{}
		// "<td>FIFO</td>"
		else if ( S_ISFIFO ( info.st_mode ) )			{}
		// "<td>Socket</td>"
		else if ( S_ISSOCK(info.st_mode ) )			{}
		// "<td>(δ֪)</td>"
		else										{}		

	}

	::closedir ( dir ) ;
#endif

	return 0 ;	
}

static void printLog(const char * sFmt, ...)
{
#define BIGLOGSIZEX 	2056
#define SMALLLOGSIZE 2048

	va_list			marker ; 		
	char 			sBuf [ BIGLOGSIZEX ] ;		
	time_t   			tmNow;   
	struct tm *		newtime ;	

	::time ( &tmNow ) ;   
	newtime   =  ::localtime(&tmNow); 		
	::memset ( sBuf, 0, BIGLOGSIZEX ) ;
	::strftime ( sBuf, 128, "%Y-%m-%d %H:%M:%S manager: ", newtime ) ;	
		
	va_start ( marker, sFmt ) ; 			
	// 2018-11-18 11:48:00 manager: 
	// |         19			|      9      | 1 |
#ifdef WIN32
	::_vsnprintf ( sBuf + 29, SMALLLOGSIZE, sFmt, marker ) ;
#else
	::vsnprintf ( sBuf + 29, SMALLLOGSIZE, sFmt, marker ) ;
#endif
	va_end ( marker ) ;

	std::cout << sBuf << std::endl ;
	
	return;
}


#endif // __PUBLIC_COMMON_20181118074800_H

