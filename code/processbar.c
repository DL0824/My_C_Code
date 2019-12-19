#include <stdio.h>                                                                                          
#include <unistd.h>
int main()
{
    char bar[101]={0};
    int i;
    const char *l="|/-\\";
    for(i=0;i<100;++i)
    {
        bar[i]='#';
        printf("[%-100s][%d%%][%c]\r",bar,i+1,l[i%4]);
        fflush(stdout);
        usleep(100000);
    }
    printf("\n");
    return 0;
}
