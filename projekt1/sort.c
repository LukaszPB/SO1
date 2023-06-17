#include "function.h"
struct comand_list* sort(struct comand_list *arg)
{
        syslog(LOG_INFO, "sort");
        if(arg == NULL)
        {
                return arg;
        }

        struct comand_list *first = arg;
        struct comand_list *p;
        struct comand_list *previous;
        struct comand_list *tmp;

        int change;

        while(1)
        {
                change = 1;
                previous = NULL;
                p = first;
                while(p->next!=NULL)
                {
                        if(p->hour*100+p->min > p->next->hour*100+p->next->min)
                        {
                                change = 0;
                                if(p == first)
                                {
                                        first = p->next;
                                }
                                if(previous == NULL)
                                {
                                        tmp = p->next->next;
                                        previous = p->next;
                                        previous->next = p;
                                        p->next = tmp;
                                }
                                else
                                {
                                        previous->next = p->next;
                                        p->next = p->next->next;
                                        previous->next->next = p;
                                        previous = previous->next;
                                }
                        }
                        else
                        {
                                previous = p;
                                p = p->next;
                        }
                }
                if(change)
                {
                        break;
                }
        }

        time_t t = time(NULL);
        struct tm *time = localtime(&t);

        int hour = time->tm_hour;
        int min = time->tm_min;

        if(p->hour*100+p->min > hour*100+min && first->hour*100+first->min < hour*100+min)
        {
                p = first;
                while(p!=NULL && p->hour*100+p->min < hour*100+min)
                {
                        previous = p;
                        p = p->next;
                }
                tmp = first;
                previous->next = NULL;
                first = p;
                while(p->next!=NULL)
                {
                        p = p->next;
                }
                p->next = tmp;
        }

        return first;
}