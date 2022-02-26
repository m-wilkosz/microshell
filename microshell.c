#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

int main()
{
	int w=1,i,j;
	char path[100],cmd[100],*arg[100],*s1[100],*s2[100],s3[100];
	while(w)
	{
		getcwd(path,100);
		printf("[%s] $ ",path);
		fgets(cmd,100,stdin);
		cmd[strlen(cmd)-1]='\0';
		if(strcmp(cmd,"exit")==0)
			w=0;
		else if(strcmp(cmd,"help")==0)
		{
			printf("================================ microshell ================================\n");
			printf("autor: Michal Wilkosz\n");
			printf("============================================================================\n");
			printf("polecenie exit konczy dzialanie programu\n");
			printf("============================================================================\n");
			printf("polecenie cd sluzy do zmiany katalogu roboczego, .. jako argument\n");
			printf("polecenia oznacza katalog nadrzedny, . oznacza aktualny katalog roboczy\n");
			printf("============================================================================\n");
			printf("funkcje systemowe fork() i execvp() umozliwiaja uruchamianie pozostalych\n");
			printf("programow znajdujacych sie w katalogach opisanych w zmiennej PATH\n");
			printf("============================================================================\n");
		}
		else if(cmd[0]=='c' && cmd[1]=='d')
		{
			if(cmd[3]=='/')
			{
				s1[0]=strtok(cmd,"/");
				i=-1;
				do
				{
					i++;
					s1[i]=strtok(NULL,"/");
				}while(s1[i]!=NULL);
			}
			else
			{
				s1[0]=strtok(path,"/");
				i=0;
				while(s1[i]!=NULL)
				{
					i++;
					s1[i]=strtok(NULL,"/");
				}
				cmd[2]='/';
				strtok(cmd,"/");
				i--;
				do
				{
					i++;
					s1[i]=strtok(NULL,"/");
				}while(s1[i]!=NULL);
			}
			i=0;
			j=0;
			while(s1[i]!=NULL)
			{
				if(!(i==0 && strcmp(s1[i],"..")==0))
				{
					if(strcmp(s1[i],"..")!=0 && strcmp(s1[i],".")!=0)
					{
						s2[j]=s1[i];
						j++;
					}
					else if(strcmp(s1[i],".")!=0)
					{
						j--;
					}
				}
				i++;
				s2[j]=NULL;
			}
			i=0;
			strcpy(s3,"/");
			if(s1[i]!=NULL)
			{
				while(s2[i]!=NULL)
				{
					strcat(s3,s2[i]);
					strcat(s3,"/");
					i++;
				}
			}
			if(chdir(s3)==-1)
				printf("error number %d: niemozliwe odnalezienie katalogu\n",errno);
			getcwd(path,100);
		}
		else
		{
			pid_t pid=fork();
			if(pid<0)
			{
				printf("error number %d: nieudane utworzenie procesu potomnego\n",errno);
			}
			else if(pid==0)
			{
				arg[0]=strtok(cmd," ");
				i=0;
				while(arg[i]!=NULL)
				{
					i++;
					arg[i]=strtok(NULL," ");
				}
				if(execvp(arg[0],arg)==-1)
				{
					printf("error number %d: niemozliwe poprawne zinterpretowanie polecenia\n",errno);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				wait(NULL);
			}
		}
	}
	return 0;
}
