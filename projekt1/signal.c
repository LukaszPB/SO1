#include "function.h"
extern volatile int flaga;

void handler(int signum)
{
    if(signum == SIGUSR1)
    {
        syslog(LOG_INFO, "signal1");
        flaga = 1;
    }
    else if(signum == SIGUSR2)
    {
        syslog(LOG_INFO, "signal2");
        flaga = 2;
    }
    else if(signum == SIGINT)
    {
        syslog(LOG_INFO, "signalINT");
        flaga = 3;
    }
}