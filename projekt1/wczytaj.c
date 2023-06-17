#include "function.h"
struct comand_list* wczytaj(char* wejscie)
{
        syslog(LOG_INFO, "wczytaj");

        FILE* plik;

        plik = fopen (wejscie, "r");
        if(plik == NULL)
        {
                syslog(LOG_ERR, "blad otwarcia pliku");
                return NULL;
        }

        struct comand_list *first = NULL;
        
        while(1)
        {
                struct comand_list *nowy = malloc(sizeof(struct comand_list));

                char napis[100];
                for(int i=0;i<100;i++)
                {
                        napis[i] = ' ';
                }
                
                int status = fscanf(plik,"%d:%d:%[^:]:%d",&(nowy->hour),&(nowy->min),napis,&(nowy->mode));

                if(status == EOF)
                {
                        syslog(LOG_INFO, "koniec wczytywania");
                        break;
                }

                int k = 0;
                int nr = 0;
                char **arguments;
                char *new_row;
                        
                for(int i=0;i<100;i++)
                {
                        if(napis[i] == ' ')
                        {
                                new_row = malloc(sizeof(char)*(i-k+1));
                                
                                for(int j=0;j<i-k;j++)
                                {
                                        new_row[j] = napis[j+k];
                                }
                                new_row[i-k] = '\0';

                                if(k == 0)
                                {
                                        nowy->task = new_row;
                                        arguments = malloc(sizeof(char *));
                                }
                                else
                                {
                                        arguments = realloc(arguments, (nr+1) * sizeof(char*));
                                }

                                arguments[nr] = new_row;

                                nr++;        
                                k = i+1;
                                        
                                if(napis[i+1] == ' ')
                                {
                                        arguments = realloc(arguments, (nr+1) * sizeof(char*));
                                        arguments[nr] = NULL;

                                        nowy->arguments = arguments;
                                        nowy->number_of_arguments = nr;
                                        
                                        break;
                                }
                        }
                }
                nowy->next = first;
                first = nowy;
        }

        fclose(plik);

        return first;
}