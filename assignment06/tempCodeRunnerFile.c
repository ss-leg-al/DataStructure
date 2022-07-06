#include <stdio.h>
#include <stdlib.h> // malloc, realloc

#include "adt_heap.h"

/* Reestablishes heap by moving data in child up to correct location heap array
*/
static void _reheapUp( HEAP *heap, int index)
{
    int parent;
    void* tmp;
    if(index)
    {
       
        parent=(index-1)/2;
        if(heap->compare(heap->heapArr[index],heap->heapArr[parent])>0)
        {
            tmp=heap->heapArr[parent];
            heap->heapArr[parent]=heap->heapArr[index];
            heap->heapArr[index]=tmp;
            _reheapUp(heap,parent);
        }
    }
    return;
}


/* Reestablishes heap by moving data in root down to its correct location in the heap
*/
static void _reheapDown( HEAP *heap, int index)
{
    void* tmp;
    void* left;
    void* right;
    int a;
    int b;
    a=heap->last;
    if((index*2+1)<=a)
    {
        left=heap->heapArr[index*2+1];
        if((index*2+2)<=a)
        {
            right=heap->heapArr[index*2+2];
        }
        else
        {
            right=NULL;
        }
        if(!right||heap->compare(left,right)>0)
        {
            b=index*2+1;
        }
        else
        {
            b=index*2+2;
        }
        if(heap->compare(heap->heapArr[index],heap->heapArr[b])<0)
        {
            tmp=heap->heapArr[index];
            heap->heapArr[index]=heap->heapArr[b];
            heap->heapArr[b]=tmp;
            _reheapDown(heap,b);
        }
    }
    return;
}



/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2))
{
    HEAP* heap;
    heap=(HEAP*)malloc(sizeof(HEAP));
    heap->capacity=capacity;
    heap->last=-1;
    heap->compare=compare;
    heap->heapArr=malloc(sizeof(void*)*capacity);
    
    return heap;
}

/* Free memory for heap
*/
void heap_Destroy( HEAP *heap)
{
    free(*(heap->heapArr));
    free(heap->heapArr);
    heap->capacity=0;
    heap->compare=NULL;
    heap->last=-1;
    free(heap);

}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert( HEAP *heap, void *dataPtr)
{
    printf("%d",heap->last);
    if(heap->last<-1)
    {
        return 0;
    }
    if(heap->last==-1)
    {   
        heap->last=0;
        heap->heapArr[heap->last]=dataPtr;
        return 1;
    }
    if((heap->last)==(heap->capacity-1))
    {
        heap->heapArr=realloc(heap->heapArr,sizeof(void*)*(heap->capacity)*2);
        heap->capacity*=2;
    }
    (heap->last)++;
    heap->heapArr[heap->last]=dataPtr;
    _reheapUp(heap,heap->last);
    return 1;
}


/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr)
{
    if(heap_Empty(heap))
    {
        return 0;
    }
    *dataOutPtr=heap->heapArr[0];
    heap->heapArr[0]=heap->heapArr[heap->last];
    (heap->last)--;
    _reheapDown(heap,0);
    return 1;
}

/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap)
{
    if ((heap->last)==-1)
    {
        return 1;
    }
    return 0;
}

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data))
{
    for(int i=0;i<=(heap->last);i++)
    {
        print_func(heap->heapArr[i]);
    }
    printf("\n");
}

