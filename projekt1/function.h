#ifndef FUNCTION_H
#define FUNCTION_H
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <syslog.h>
#include <fcntl.h>
struct comand_list
{
        int hour,min,mode,number_of_arguments;
        char *task;
        char **arguments;
        struct comand_list* next;
};
struct comand_list* wczytaj(char *);
struct comand_list* sort(struct comand_list*);
void zwolnij_pamiec(struct comand_list*);
void handler(int);
#endif