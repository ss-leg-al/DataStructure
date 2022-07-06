#include <stdio.h>
#include <stdlib.h>
#include <string.h>    
#include <assert.h>

int main(void)
{
    char s[50]="Isabella,F,22306";
    char *k;
    int i;
    char name[1024];
    char sex;
    int freq;
    for(k=s;;k++)
        {   
            if(!*k)
                break;
            if(*k==',')
                *k='\t';
        }
    sscanf(s,"%s\t%c\t%d",name,&sex,&freq);
    printf("%s\t%c\t%d",name,sex,freq);
    return 0;
    
}