#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <daemon.h>
#include <signal.h>
namespace ez{

int becomeDaemon(int flags)
{
    int maxfd,fd;

    switch(fork()){             //变为后台进程(background process)
        case -1: return -1;
        case 0 : break;
        default:
            _exit(0);
    }

    if(setsid() ==  -1)     //成为新会话的首进程(leader)
        return -1;

    switch(fork()){         //确保不是首进程
        case -1: return -1;
        case 0 : break;
        default:
            _exit(0);
    }

    if(!(flags & BD_NO_UMASK))      //清除掩码，获得权限
        umask(0);

    if(!(flags & BD_NO_CHDIR))  
        chdir("/");

    if(!(flags & BD_NO_CLOSE_FILES)){   //关闭所有打开文件
        maxfd = sysconf(_SC_OPEN_MAX);
        if(maxfd == -1)                 
            maxfd = BD_MAX_CLOSE;
        
        for(fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if(!(flags & BD_NO_REOPEN_STD_FDS)){
        close(STDIN_FILENO);        //重新打开标准输入 ，重定向到/dev/null
    
        fd = open("/dev/null",O_RDWR);
        if(fd != STDERR_FILENO)
            return -1;
        if(dup2(STDIN_FILENO,STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if(dup2(STDIN_FILENO,STDERR_FILENO) != STDERR_FILENO)
            return -1;
    
    }

    signal(SIGCHLD,SIG_IGN);
    return 0;


}
}