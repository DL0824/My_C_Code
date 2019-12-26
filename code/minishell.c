#include <stdio.h>
#include <sys/wait.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

char g_command[1024];

int Get_Command()
{
    memset(g_command,'0',sizeof(g_command));
    printf("[minishell@localhost]$");
    fflush(stdout);
   if(!fgets(g_command,sizeof(g_command)-1,stdin))
   {
       perror("fgets error");
       return -1;
   }
    return 0;
}

void execommand(char **argv)
{
    if(!argv[0])
    {
        return;
    }

    pid_t pid=fork();
    if(pid<0)
    {
        perror("fork error");
        return;
    }
    else if(pid==0)
    {
        if(-1==execvp(argv[0],argv))
        {
            printf("未找到命令\n");
        }
        exit(0);
    }
    else
    {
        waitpid(pid,NULL,0);
    }
}

int DealCommand(char *command)
{//切割输入
    if(!command||*command=='\0')
    {
        printf("DealCommand failed\n");
        return -1;
    }
    char *argv[32]={0};
    int argc=0;
    while(*command)
    {
        if(!isspace(*command))
        {
            argv[argc++]=command;
            while(!isspace(*command)&&*command!='\0')
            {
                command++;
            }
            *command='\0';
        }
        command++;
    }
    argv[argc]=NULL;

    execommand(argv);

    return 0;
}


int main()
{
    while(1)
    {
        if(Get_Command()!=0)
        {
            continue;
        }
        DealCommand(g_command);

    }
    return 0;
}
