#include "function.h"
void zwolnij_pamiec(struct comand_list *p)
{
    for(int i=0;i<p->number_of_arguments;i++)
    {
        free(p->arguments[i]);
    }
    free(p->arguments);
    free(p);
    syslog(LOG_INFO, "zwolniono pamiec");
}