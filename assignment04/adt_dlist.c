#include <stdlib.h> // malloc

#include "adt_dlist.h"

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, void *dataInPtr)
{
	NODE* tmp=(NODE*)malloc(sizeof(NODE));
	if(!tmp)
	{
		return 0;
	}
	tmp->dataPtr=dataInPtr;
	tmp->llink=NULL;
	tmp->rlink=NULL;
	if (emptyList(pList))
	{
		pList->head=tmp;
		pList->rear=tmp;
	}
	else if(pPre==NULL)
	{
		(pList->head)->llink=tmp;
		tmp->rlink=pList->head;
		pList->head=tmp;
	}
	else if(pPre==(pList->rear))
	{
		pPre->rlink=tmp;
		tmp->llink=pPre;
		pList->rear=tmp;
	}
	else
	{
		tmp->rlink=pPre->rlink;
		(pPre->rlink)->llink=tmp;
		pPre->rlink=tmp;
		tmp->llink=pPre;
	}
	
	(pList->count)++;
	return 1;
		
}

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, void **dataOutPtr)
{
	*dataOutPtr=pLoc->dataPtr;
	if(pPre==NULL)
	{
		pList->head=pLoc->rlink;
		(pLoc->rlink)->llink=NULL;
	}
	else if(pPre==(pList->rear))
	{
		pList->rear=pPre;
		pPre->rlink=NULL;
	}
	else
	{
		pPre->rlink=pLoc->rlink;
		(pLoc->rlink)->llink=pPre;
	}
	(pList->count)--;
	free(pLoc);
}
/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, void *pArgu)
{
	*pPre=NULL;
	*pLoc=pList->head;
	int a;
	int i;
	for(i=0;i<(pList->count);i++)
	{	
		a=pList->compare(pArgu,(*pLoc)->dataPtr);
		if(a==0)
		{
			return 1;
		}
		else if(a<0)
		{	
			return 0;
		}
		else
		{
			*pPre=*pLoc;
			*pLoc=(*pLoc)->rlink;
		}
	}
	
	return 0;
}
/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( int (*compare)(const void *, const void *))
{
	LIST* a=(LIST*)malloc(sizeof(LIST));
	if(a)
	{
		a->count=0;
		a->head=NULL;
		a->rear=NULL;
		a->compare=compare;
		return a;
	}
	else
	{
		return NULL;
	}
}
 
/* Deletes all data in list and recycles memory
*/
void destroyList( LIST *pList, void (*callback)(void *))
{
	
	NODE *a;
	if(pList)
	{
		while((pList->count)>0)
		{	
			callback(pList->head->dataPtr);
			a=pList->head;
			pList->head=(pList->head)->rlink;
			pList->count--;
			free(a);
		}
		free(pList);
	}

}
/* Inserts data into list
	return	0 if overflow
			1 if successful
			2 if duplicated key
*/
int addNode( LIST *pList, void *dataInPtr, void (*callback)(const void *, const void *))
{	
	NODE *pPre;
	NODE *pLoc;

	if(_search(pList,&pPre,&pLoc,dataInPtr)==1)
	{	
		callback(pLoc->dataPtr,dataInPtr);
		return 2;
	}	
	else
	{
		if(_insert(pList,pPre,dataInPtr)==1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, void *keyPtr, void **dataOutPtr)
{
	NODE *pPre;
	NODE *pLoc;
	int a;
	a=_search(pList,&pPre,&pLoc,keyPtr);
	if(a)
	{
		_delete(pList,pPre,pLoc,dataOutPtr);
	}
	return a;
}
/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
int searchList( LIST *pList, void *pArgu, void **dataOutPtr)
{
	NODE *pPre;
	NODE *pLoc;
	int a;
	a=_search(pList,&pPre,&pLoc,pArgu);
	if(a)
	{
		*dataOutPtr=pLoc->dataPtr;
	}
	else
	{
		*dataOutPtr=NULL;
	}
	return a;

}

/* returns number of nodes in list
*/
int countList( LIST *pList)
{
	return pList->count;
}

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList)
{
	return (pList->count)==0;
}

/* traverses data from list (forward)
*/
void traverseList( LIST *pList, void (*callback)(const void *))
{
	NODE *a=pList->head;
	int i;
	for(i=0;i<(pList->count);i++)
	{
		callback(a->dataPtr);
		a=a->rlink;
	}
}

/* traverses data from list (backward)
*/
void traverseListR( LIST *pList, void (*callback)(const void *))
{
	NODE *a=pList->rear;
	int i;
	for(i=0;i<(pList->count);i++)
	{
		callback(a->dataPtr);
		a=a->llink;
	}
}