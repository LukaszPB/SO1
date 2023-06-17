#include "function.h"
volatile int flaga;

int main (int argc, char *argv[])
{
    openlog ("projekt1_log",LOG_PID,0);
    syslog (LOG_INFO, "uruchomiono program");

    flaga = 0;

    struct comand_list* first = wczytaj(argv[1]);
    first = sort(first);

    signal(SIGINT, handler);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);

    /* Our process ID and Session ID */
    pid_t pid, sid;
    int status;

    /* Fork off the parent process */
    pid = fork();

    if (pid < 0) 
    {
            perror("fork() error: ");
            syslog(LOG_ERR, "fork() error");
            exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
    we can exit the parent process. */
    if (pid > 0) 
    {
            exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);
                
    /* Open any logs here */        
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) 
    {
            perror("setsid() error: ");
            syslog(LOG_ERR, "setsid() error");
            exit(EXIT_FAILURE);
    }
        
    /* Change the current working directory */
    
    if ((chdir("/")) < 0) 
    {
            perror("chdir() error: ");
            syslog(LOG_ERR, "chdir() error");
            exit(EXIT_FAILURE);
    }
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* The Big Loop */

    FILE* wyjscie;
    wyjscie = fopen(argv[2], "a");

    time_t t;
    struct tm *czas;
    int hour,min;
    
    struct comand_list* tmp;

    while (1) 
    {
        if(flaga == 1)
        {
            while(first!=NULL)
            {
                tmp = first;
                first = first->next;
                zwolnij_pamiec(tmp);
            }
            first = wczytaj(argv[1]);
            first = sort(first);
            flaga = 0;
        }
        else if(flaga == 2)
        {
            struct comand_list* p = first;
            while(p!=NULL)
            {
                syslog (LOG_INFO, "%d:%d %s %d argumenty: ",p->hour,p->min,p->task,p->mode);
                p = p->next;
            }
            flaga = 0;
        }
        else if(flaga == 3)
        {
            break;
        }
        
        t = time(NULL);
        czas = localtime(&t);

        hour = czas->tm_hour;
        min = czas->tm_min;

        if(hour == first->hour && min == first->min)
        {
            pid = fork();

            if(pid < 0)
            {
                syslog (LOG_ERR, "fork() error");
                _exit(EXIT_FAILURE);
            }
            else if(pid == 0)
            {
                dup2(fileno(wyjscie), STDOUT_FILENO);
                fclose(wyjscie);

                syslog(LOG_INFO, "uruchomiono zadanie: %s",first->task);
                execvp(first->task,first->arguments);
                _exit(EXIT_FAILURE);
            }
            else
            {
                if(waitpid (pid, &status, 0) != pid)
                {
                    syslog (LOG_ERR, "waitpid() error");
                    _exit(EXIT_FAILURE);
                }
                syslog (LOG_INFO, "kod wyjscia zadania: %d",WEXITSTATUS(status));
            }

            tmp = first;
            first = first->next;
            zwolnij_pamiec(tmp);

            if(first == NULL)
            {
                break;
            }
        }
        else
        {
            sleep(5); /* wait 5 second */
        }
    }

    syslog (LOG_INFO, "zakonczono program");
    closelog ();
    exit(EXIT_SUCCESS);
}