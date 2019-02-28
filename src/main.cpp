#include <fcntl.h> 
#include <time.h>
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h> 
#include <signal.h>
#include <unistd.h> 
#include <signal.h> 
#include <termios.h> 
#include <sys/stat.h> 
#include <sys/time.h> 
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/resource.h>
#include "server_manager.h"

#define TIMEIDLE 200 
#define TIMEDELAY 6 
#define INSTANCE_FILE	"./nvrserver.pid"
#define CNPVERSION		"Version 0.0.1 Build ( 2018.11.11 )"

// �Ƿ��ǵ���ģʽ
bool				g_bIsExit ;
bool				g_bIsDebug ;

void set_timer()
{
	struct itimerval itv, oldtv;
	itv.it_interval.tv_sec = 5;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 5;
	itv.it_value.tv_usec = 0;

	if ( ServerManager .isRun () )
	{
		::setitimer ( ITIMER_REAL, &itv, &oldtv ) ;
	}
}

void setup_daemon(void)
{
	int i;
	for (i = 0; i<NOFILE; ++i)
		close(i);

	switch (fork())
	{
	case -1:
	{
		perror("setup_daemon(), 1st fork()");
		exit(2);
	}

	default:
		exit(0);
			
	case 0:
		if (setsid()==-1)
		{
			perror("setup_daemon(), setsid()");
			exit(3);
		}

		switch (fork())
		{
		case -1:
		{
			perror("setup_daemon(), 2nd fork()");
			exit(3);
		}

		default:
			exit(0);

		case 0:
			umask(0);
			/* and return with daemon set up */
			return;
		}
	}
}

// ��ֹ��ʬ����
void signal_child(int sig)
{
	pid_t 	pid ;
	int		stat ;

	while ( true )
	{
		//WNOHANG ��������ʽ
		pid = ::waitpid ( -1, &stat, WNOHANG ) ;
		if ( pid == 0 ||( pid ==-1 &&errno!= EINTR ) )
		{
			//break;
			return ;
		}
		
		ServerManager .taskExit ( pid, stat ) ;

		//ToplevelStudio::Log .Debug ( FILELINE, "sig_chld - has child process exit!" ) ;
	}
   
	return ;
}

// �źŴ�����
void signal_handler(int sig)
{
	//ToplevelStudio::Log .Debug ( FILELINE, "fcmserver has received signal ( %d )", sig  ) ;
	
	switch ( sig )
	{
	// û���κ��ź��յ�
	case 0		: 
		{
		}		
		break;	

	// �����û��ź�
	case SIGUSR1:
		{
			// ��ӡ�û���Ϣ
			ServerManager .dump () ;
		}
		break ;
	
	// ��������
	case SIGHUP	: 
		{
			// ��¼��������������־
			// ToplevelStudio::Log .Debug ( FILELINE, "fcmserver now restarting ... ..."  ) ;
			//ServerManager .Inital () ;
			//ServerManager .UnInit () ;
		}
		break;	

	// �رճ���
	case SIGINT :
		{
			if ( g_bIsExit == false )
			{
				g_bIsExit	= 	true ;
				//ServerManager .m_event .Wakeup () ;
			}			
			::unlink ( INSTANCE_FILE ) ;
		}
		break ;

	// �ӽ����˳�
	case SIGCHLD :
		{
			signal_child ( SIGCHLD ) ;
		}
		break ;

	// 
	case SIGKILL :
	case SIGTERM :	
		{
			::unlink ( INSTANCE_FILE ) ;
		}
		break ;
		
	// ȱʡ����
	default		:
		{}
		break ;
	}
	
}

void build_sigset(sigset_t * sigset)
{
	::sigemptyset ( sigset ) ;
	// ::sigaddset ( sigset, SIGPIPE ) ;
	::sigaddset ( sigset, SIGINT ) ;
	::sigaddset ( sigset, SIGHUP ) ;
	::sigaddset ( sigset, SIGTERM ) ;
	// ::sigaddset ( sigset, SIGALRM ) ;
	::sigaddset ( sigset, SIGUSR1 ) ;
	::sigaddset ( sigset, SIGCHLD ) ;

	//��ͼ����SIGSEGV�ź�
	// ::sigaddset ( sigset, SIGSEGV ) ; 
}

void * signal_thread(void * param)
{
	::pthread_detach ( ::pthread_self () ) ;
	
	//����SIGQUIT�źţ���������յ����źź��˳�
	// ::signal ( SIGQUIT, signal_handler ) ;

	sigset_t 	sigset ;

	build_sigset ( &sigset ) ;

	while ( false == g_bIsExit )
	{
#ifdef USE_SIGWAIT
		int sig_no ;
		int ret_no = ::sigwait ( &sigset, &sig_no ) ;
		if ( ret_no == 0 )
		{
			signal_handler ( sig_no ) ;
		}
		else
		{
			printf("sigwait() failed, errno: %d(%s)!\n", ret_no, strerror(ret_no));
		}
#else
		siginfo_t sig_info ;

		if ( ::sigwaitinfo ( &sigset, &sig_info )  != -1 ) 
		{
			signal_handler ( sig_info .si_signo ) ;
		}
		else
		{
			//�������ź��ж�
			if ( errno == EINTR )
			{
				printf ( "sigwaitinfo() was interrupted by a signal handler!\n" ) ;
			}
			else
			{
				printf ( "sigwaitinfo() was interrupted by a signal handler1!\n" ) ;
				// printf("sigwaitinfo() failed, errno: %d(%s)!\n", errno, strerror(errno));
			}
		}
	}
#endif

	return 0 ;
}

// �����źŴ���
void setup_signal()
{
	// EPIPE is easier to handle than a signal
	::signal ( SIGPIPE, SIG_IGN ) ;

	sigset_t 	sigset ;

	build_sigset ( &sigset ) ;
 	//�����߳��ź�������
 	::pthread_sigmask ( SIG_BLOCK, &sigset, NULL ) ;
	
 	//����һ�������̣߳����̸߳����źŵ�ͬ������
 	pthread_t 	threadID ;
	::pthread_create ( &threadID, NULL, signal_thread, NULL ) ;	

	/*
	// handle signals
	::signal ( SIGINT, signal_handler ) ;
	::signal ( SIGHUP, signal_handler ) ;
	::signal ( SIGTERM, signal_handler ) ;

	// Alarm signals
	::signal ( SIGALRM, alrm_handler ) ;

	// user1 signals
	::signal ( SIGUSR1, signal_handler ) ;

	// child signals
	::signal ( SIGCHLD, signal_child ) ;	
	*/
}

/////////////////////////////////////////////
//
// ��ǰ�Ƿ��з�����ʵ��������
//
/////////////////////////////////////////////
bool IsHaveInstance()
{
    int lock_fd = ::open ( INSTANCE_FILE, O_RDWR | O_CREAT, 0640 ) ;
    if ( lock_fd < 0 )
    {
	perror( "lock file fail" );
	::printf("\n\033[1m\033[0;31m"
		       "******************************************************************\n"
		       "                     server boot fail!\n"
		       "                     open lock file fail!\n"
		       "******************************************************************\033[0m\n");
    	return true;
    }

    struct flock fk;
    fk.l_type =  F_WRLCK;
    fk.l_whence = SEEK_SET;
    fk.l_start = 0;
    fk.l_len = 0;

    if ( ::fcntl (lock_fd, F_SETLK, &fk) < 0 )
    {
	::perror ( "lock file fail" );
    	::printf ( "\n\033[1m\033[0;31m"
		       "******************************************************************\n"
		       "                     server boot fail!\n"
		       "  You have a running fcmserver instance, please kill it first!\n"
		       "******************************************************************\033[0m\n" ) ;
    	return true;
    }

    return false;
}

//////////////////////////////////////
//
// ��ӡ������Ϣ
//
//////////////////////////////////////
void usage()
{
	::printf ( "\n"
		"****************************************************************\n"
		"	Wellcom to nvrserver, Author : CaiWenFeng say hello to you!\n"
		"****************************************************************\n\n"
		"usage : \n"
		"	nvrserver [arguments]\n\n"
		"Arguments:\n"
		"	-b		daemon mode run!\n"
		"	-d		debug mode!\n"
		"	-h		printf help!\n"
		"	-m		printf machine code!\n"
		"	-v		print server version!\n"
		"	-r		input register code!\n"
		"	-l		specify log file and record!\n"
		"	-0		write log info to terminal!\n"
		"	-1		write log to file (default : log.txt)!\n"
		"	-2		write log to terminal not have file&line info!\n"
		"	-3		write to file not have file&line info!\n"
		"	-4		write syslog file!(daemon mode)\n"
		"	-5		write log (date as file name)\n"
		"\n" ) ;
}

// **************************************************
//
// ��������
//
// **************************************************
int main(int argc, char** argv) 
{

	int		opt;
	bool		bDaemon = false ;

	g_bIsExit	=	false ;
       opterr 		=	0 ;

	// ������������ð��,  ��ѡ���������һ������
	// ������������˫ð��, ��ѡ��������������������пո�
       while ( ( opt = ::getopt(argc,argv,"bdhvmr:l:012345") ) != -1)
	{
       	switch(opt)
              {
              // ���ػ����̵ķ�ʽ����
              case 'b':
			bDaemon = true ;
                     break ;

		// �Ե��Է�ʽ����
		case 'd' :
              	g_bIsDebug = true ;
	              break ;

		// ��ָ������־�������
		case 'l':
			//ToplevelStudio::Log .SetFileName( optarg ) ;
                     break ;

		// ��ӡ��ǰ�������İ汾
		case 'v':
			::printf ( "Version 7.0 Build ( 2009.03.27 )\n" ) ;
			::exit ( 0 ) ;
			break ;		

		// ָ����־�������
		case '0':			
		case '1':			
		case '2':			
		case '3':		
		case '4':		
		case '5':
			break ;

		// ��������δ֪�ַ�
		case 'h' :
		case '?' :
			usage () ;
			::exit ( 0 ) ;
			break ;
      		}
	}

	// �жϵ�ǰ�Ƿ��з�����ʵ��������
	if ( IsHaveInstance () )
	{
		return 0 ;
	}

	// ���ػ����̵ķ�ʽ����
	if ( bDaemon )
	{
		//ToplevelStudio::Log .SetLogOutput ( ToplevelStudio::logInfo ) ;
		setup_daemon () ;
	}

	// �����½�����
	::setpgid ( 0, 0 ) ;

	// �����źŴ���
	setup_signal () ;

	// ���������ļ��
	struct rlimit	limit ;
	::getrlimit ( RLIMIT_NOFILE, &limit ) ;

	// ����������ó������ƣ��������׳�Too many open files ������	
	limit.rlim_cur = 500000 ;
       limit.rlim_max = 500000 ;	   
       ::setrlimit ( RLIMIT_NOFILE, &limit ) ;

	// ����core  �ļ���С���������
	::getrlimit ( RLIMIT_CORE, &limit ) ;
	 limit.rlim_cur = RLIM_INFINITY;
        limit.rlim_max = RLIM_INFINITY;
        ::setrlimit ( RLIMIT_CORE, &limit ) ;	

	ServerManager .inital () ;	
	ServerManager .wait () ;		
	ServerManager .unInit () ;	
	// ToplevelStudio::Log .Info ( FILELINE, "main proccess has been [ \033[1m\033[0;31mexit\033[0m ]!" ) ;	
	::unlink ( INSTANCE_FILE ) ;

	//char		szCmdText [512]	=	{0} ;
	//::memset ( szCmdText, 0, 512 ) ;
	//::sprintf ( szCmdText, "rm -rf %s", INSTANCE_FILE ) ;
	//::system ( szCmdText ) ;		
	
	return 0;
	
} 

