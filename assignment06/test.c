int heap_Insert( HEAP *heap, void *dataPtr)
{
    printf("%d",heap->last);
    if(heap->last==-1)
    {   
        (heap->last)++;
        heap->heapArr[heap->last]=dataPtr;
        return 1;
    }
    if((heap->last)==(heap->capacity-1))
    {
        return 0;
    }
    if((heap->last)==(heap->capacity))
    {
        heap->heapArr=realloc(heap->heapArr,sizeof(void*)*(heap->capacity)*2);
        heap->capacity*=2;
    }
    (heap->last)++;
    heap->heapArr[heap->last]=dataPtr;
    _reheapUp(heap,heap->last);
    return 1;
}

int heap_Insert( HEAP *heap, void *dataPtr)
{
    printf("%d",heap->last);
    if(heap->last==-1)
    {   
        (heap->last)++;
        heap->heapArr[heap->last]=dataPtr;
        return 1;
    }
    if((heap->last)==(heap->capacity-1))
    {
        heap->heapArr=realloc(heap->heapArr,sizeof(void*)*(heap->capacity)*2);
        heap->capacity*=2;
        (heap->last)++;
        heap->heapArr[heap->last]=dataPtr;
        _reheapUp(heap,heap->last);
        return 0;
    }
    else
    {
    (heap->last)++;
    heap->heapArr[heap->last]=dataPtr;
    _reheapUp(heap,heap->last);
    return 1;
    }
}

static void _reheapUp( HEAP *heap, int index)
{
    int parent;
    void** a;
    void* b;
    if(index)
    {
        a=heap->heapArr;
        parent=(index-1)/2;
        if(heap->compare(a[index],a[parent])>0)
        {
            b=a[parent];
            a[parent]=a[index];
            _reheapUp(heap,parent);
        }
    }
    return;
}
