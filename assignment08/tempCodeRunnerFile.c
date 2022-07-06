#include <stdio.h>
#include <stdlib.h>	// malloc
#include <string.h>	// strdup
#include <ctype.h>	// isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	int 			index; // -1 (non-word), 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void);

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root);

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index);

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str);

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[]);

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[]);

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]);

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size);

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[]);

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	TRIE *trie;
	TRIE *permute_trie;
	char *dic[100000];

	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p;
	int index = 0;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	trie = trieCreateNode(); // original trie
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	while (fscanf( fp, "%s", str) != EOF)
	{	
		ret = trieInsert( trie, str, index);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);
			
			for (int i = 0; i < num_p; i++)
				trieInsert( permute_trie, permuterms[i], index);
			
			clear_permuterms( permuterms, num_p);
			
			dic[index++] = strdup( str);
			
		}
		
	}
	
	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) != EOF)
	{
		// wildcard search term
		if (strchr( str, '*')) 
		{	
			trieSearchWildcard( permute_trie, str, dic);
		}
		// keyword search
		else 
		{
			ret = trieSearch( trie, str);
			if (ret == -1) printf( "[%s] not found!\n", str);
			else printf( "[%s] found!\n", dic[ret]);
		}
		printf( "\nQuery: ");
	}

	for (int i = 0; i < index; i++)
		free( dic[i]);
	
	trieDestroy( trie);
	trieDestroy( permute_trie);
	
	return 0;
}

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void)
{
	TRIE* node;
	node=(TRIE*)malloc(sizeof(TRIE));
	if(node)
	{
		node->index=-1;
		for(int i=0;i<MAX_DEGREE;i++)
		{
			node->subtrees[i]=NULL;
		}
		return node;
	}
	else
	{
		return NULL;
	}
}

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root)
{
	if(root)
	{
		for(int i=0;i<MAX_DEGREE;i++)
		{
			trieDestroy(root->subtrees[i]);
		}
		free(root);
	}
}

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자와 EOW 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index)
{
	TRIE* tmp;
	tmp=root;
	while(*str)
	{
		if(isupper(*str))
		{
			if(tmp->subtrees[getIndex(tolower(*str))]==NULL)
			{
				tmp->subtrees[getIndex(tolower(*str))]=trieCreateNode();
			}
		}
		else
		{
			if(tmp->subtrees[getIndex(*str)]==NULL)
			{
				tmp->subtrees[getIndex(*str)]=trieCreateNode();
			}
		}
		tmp=tmp->subtrees[getIndex(*str)];
		str++;
	}
	if((tmp->index)!=-1)
	{
		return 0;
	}
	else
	{
		tmp->index=dic_index;
		return 1;
	}

}

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str)
{
	if(!root)
	{
		return -1;
	}
	TRIE* tmp;
	tmp=root;
	while(*str)
	{
		tmp=tmp->subtrees[getIndex(*str)];
		if(!tmp)
		{
			return -1;
		}
		str++;
	}
	return tmp->index;
}

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root, char *dic[])
{	
	int k=root->index;
	if(k!=-1)
	{

		printf("%s\n",dic[k]);
	
	}
	for(int i=0;i<MAX_DEGREE;i++)
	{	
		if(root->subtrees[i]==NULL)
		{
			continue;
		}
		else
		{
			trieList(root->subtrees[i],dic);
		}
	}

}

/* prints all entries starting with str (as prefix) in trie
	ex) "abb" -> "abbas", "abbasid", "abbess", ...
	this function uses trieList function
*/
void triePrefixList( TRIE *root, char *str, char *dic[])
{
	TRIE* tmp;
	tmp=root;
	while(*str)
	{
		tmp=tmp->subtrees[getIndex(*str)];
		str++;
	}
	trieList(tmp,dic);
}

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[])
{	
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
        permuterms[i]=strdup(word);
    }
    free(word);
	return strlen(str)+1;
}

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size)
{
	for(int i=0;i<size;i++)
	{
		free(permuterms[i]);
	}
}

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	this function uses triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str, char *dic[])
{
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
        if(word[strlen(str)]=='*')
		{
			break;
		}
    }
	word[strlen(str)]='\0';
	triePrefixList(root,word,dic);
    free(word);
}