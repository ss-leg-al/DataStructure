#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

int main(void)
{   
    char* permuterms[100];
    char* str="Hello";
    char dollar[]="$";
    char tmp;
    char *word=(char*)malloc(sizeof(char)*100);
    word=strdup(str); 
    strcat(word,dollar);
    for(int i=0;i<strlen(str)+1;i++)
    {
        tmp=word[0];
        for(int j=0;j<strlen(str)+1;j++)
        {
            word[j]=word[j+1];
        }
        word[strlen(str)]=tmp;
        printf("%s\n",word);
        //permuterms[i]=strdup(word);
    }
    printf("%d",strlen(word));
    //printf("%s",permuterms[1]);
   
}