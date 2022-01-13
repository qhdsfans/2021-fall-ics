#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.  When you hand
 * in, remove the #define DEBUG line. */
//#define DEBUG
#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

#define mydebug

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* def DRIVER */

#define WSIZE 4 /* Word and header/footer size (bytes) */
#define DSIZE 8 /* Doubleword size (bytes) */

#define CHUNKSIZE (1 << 11) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size_t)(size) + (ALIGNMENT - 1)) & ~0x7)

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp)-WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp)-WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp)-GET_SIZE(((char *)(bp)-DSIZE)))

/* Given block ptr bp, compute size/pred/succ */
#define SIZE(bp) (GET_SIZE(HDRP(bp)))
#define PREV_SIZE(bp) (GET_SIZE((char *)(bp)-DSIZE))
#define NEXT_SIZE(bp) (GET_SIZE((char *)(bp) + SIZE(bp) - WSIZE))
#define ALLOC(bp) (GET_ALLOC(HDRP(bp)))
#define PREV_ALLOC(bp) (GET_ALLOC((char *)(bp)-DSIZE))
#define NEXT_ALLOC(bp) (GET_ALLOC((char *)(bp) + SIZE(bp) - WSIZE))
#define PRED(bp) ((char *)(bp)-GET(bp))
#define SUCC(bp) ((char *)(bp) + GET((char *)(bp) + WSIZE))
#define succ(bp) *(char **)((char *)bp + WSIZE)

/* Given block ptr bp, change pred/succ */
#define PUT_PRED(bp, prev) PUT(bp, (unsigned int)((char *)(bp) - (char *)(prev)))
#define PUT_SUCC(bp, succ) PUT((char *)(bp) + WSIZE, (unsigned int)((char *)(succ) - (char *)(bp)))

static char *heap_listp = NULL;
static size_t *list_head; /* point to every list head */
static size_t *list_tail; /* point to every list tail */

static void *extend_heap(size_t asize);
static void *place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static int getindex(size_t size);
static void *insert_block(void *bp);
static void remove_block(void *bp);

void mm_checkheap(int lineno);
static void printblock(void *bp);
static int checkblock(void *bp);
static void checklist(void *bp, size_t size);
static void printlist(void *bp, long size);

#define SIZE1 (1 << 4)
#define SIZE2 (1 << 5)
#define SIZE3 (1 << 6)
#define SIZE4 (1 << 7)
#define SIZE5 (1 << 8)
#define SIZE6 (1 << 9)
#define SIZE7 (1 << 10)
#define SIZE8 (1 << 11)
#define SIZE9 (1 << 12)
#define SIZE10 (1 << 13)
#define SIZE11 (1 << 14)
#define SIZE12 (1 << 15)
#define SIZE13 (1 << 16)
#define SIZE14 (1 << 17)
#define SIZE15 (1 << 18)

/*
 * mm_init - Called when a new trace starts
 * very similar to the text book
 */
int mm_init(void)
{
    /* Create the initial empty heap */
    /* use 40 * DSIZE to store the seglist*/
    if ((heap_listp = mem_sbrk(32 * DSIZE + 4 * WSIZE)) == (void *)-1)
        return -1;

    list_head = (size_t *)heap_listp;
    list_tail = (size_t *)(heap_listp + 16 * DSIZE);
    int i;
    /* init every list*/
    for (i = 0; i < 16; ++i)
    {
        list_head[i] = list_tail[i] = (size_t)NULL;
    }
    heap_listp += 32 * DSIZE;

    PUT(heap_listp, 0);                          /* Alignment padding */
    PUT(heap_listp + 1 * WSIZE, PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + 2 * WSIZE, PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + 3 * WSIZE, PACK(0, 1));     /* Epilogue header */
    heap_listp += 2 * WSIZE;

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(CHUNKSIZE) == NULL)
        return -1;
    return 0;
}

/*
 * malloc - Apply a new space
 * very similar to the text book
 */
void *malloc(size_t size)
{
    size_t asize, extendsize;
    char *bp;

    /* Adjust block size  */
    if (size <= DSIZE)
        asize = 2 * DSIZE;
    else
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL)
    {
        bp = place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize)) == NULL)
    {
        return NULL;
    }
    place(bp, asize);
#ifdef mydebug
    mm_checkheap(1);
#endif
    return bp;
}

/*
 * free - free a allocted block
 * very similar to the text book
 */
void free(void *bp)
{
    if (bp == NULL)
        return;

    size_t size = SIZE(bp);

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    /* first insert,then coalesce*/
    coalesce(insert_block(bp));
#ifdef mydebug
    mm_checkheap(1);
#endif
}

/*
 * realloc - Change the size of the block by mallocing a new block,
 *      copying its data, and freeing the old block.
 * 1.When ptr==HEAP_NIL, call malloc(size), when size==0, free ptr.
 * 2.if size>0, compare new size and old size,
 *(1) if newsize < oldsize, check if the size of the rest space>=16,
 *       if so, divide the block into 2 pieces; else remain the old block size 
 *(2) else, check if the next or the previous space is free, if so, coalesce the block and the next/previous block 
 *
 */

void *realloc(void *ptr, size_t size)
{
    size_t oldsize, asize, freesize;
    char *bpnext, *next;
    void *newptr;

    /* If oldptr is NULL, then this is just malloc. */
    if (ptr == NULL)
        return malloc(size);

    /* If size == 0 then this is just free, and we return NULL. */
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }

    oldsize = SIZE(ptr);
    asize = ALIGN(size + DSIZE);
    if (oldsize >= asize)
    {
        if (oldsize - asize >= 16)
        {
            if (!NEXT_ALLOC(ptr))
            { /* if next block is free, coalesce and split*/
                oldsize += NEXT_SIZE(ptr);
                remove_block(NEXT_BLKP(ptr));
            }
            PUT(HDRP(ptr), PACK(asize, 1));
            PUT(FTRP(ptr), PACK(asize, 1));
            bpnext = NEXT_BLKP(ptr);
            PUT(HDRP(bpnext), PACK(oldsize - asize, 0));
            PUT(FTRP(bpnext), PACK(oldsize - asize, 0));
            insert_block(bpnext);
        }
        else
        {
            PUT(HDRP(ptr), PACK(oldsize, 1));
            PUT(FTRP(ptr), PACK(oldsize, 1));
        }
#ifdef mydebug
        mm_checkheap(1);
#endif
        return ptr;
    }
    else if (!NEXT_ALLOC(ptr) && NEXT_SIZE(ptr) + oldsize >= asize)
    {
        next = NEXT_BLKP(ptr);
        remove_block(next);
        if (SIZE(next) + oldsize - asize >= 16)
        {
            freesize = SIZE(next) + oldsize - asize;
            PUT(HDRP(ptr), PACK(asize, 1));
            PUT(FTRP(ptr), PACK(asize, 1));
            bpnext = NEXT_BLKP(ptr);
            PUT(HDRP(bpnext), PACK(freesize, 0));
            PUT(FTRP(bpnext), PACK(freesize, 0));
            insert_block(bpnext);
        }
        else
        {
            PUT(HDRP(ptr), PACK(SIZE(next) + oldsize, 1));
            PUT(FTRP(ptr), PACK(SIZE(next) + oldsize, 1));
        }
#ifdef mydebug
        mm_checkheap(1);
#endif
        return ptr;
    }
    /* else we must do malloc directly*/
    newptr = malloc(size);
    memcpy(newptr, ptr, size);
    free(ptr);
#ifdef mydebug
    mm_checkheap(1);
#endif
    return newptr;
}

/*
 *   calloc - you may want to look at mm-naive.c
 *   This function is not tested by mdriver, but it is
 *   needed to run the traces.
 */
void *calloc(size_t nmemb, size_t size)
{
    size_t bytes = nmemb * size;
    void *newptr;

    newptr = malloc(bytes);
    memset(newptr, 0, bytes);

    return newptr;
}

static void *coalesce(void *bp)
{
    size_t prev = PREV_ALLOC(bp);
    size_t next = NEXT_ALLOC(bp);
    size_t size = SIZE(bp);

    if (prev && next)
        return bp;
    else if (prev && !next)
    {
        size += SIZE(NEXT_BLKP(bp));
        remove_block(NEXT_BLKP(bp));
        remove_block(bp);
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert_block(bp);
    }

    else if (!prev && next)
    {
        size += SIZE(PREV_BLKP(bp));
        remove_block(PREV_BLKP(bp));
        remove_block(bp);
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        insert_block(PREV_BLKP(bp));
        bp = PREV_BLKP(bp);
    }

    else
    {
        size += SIZE(PREV_BLKP(bp)) + SIZE(NEXT_BLKP(bp));
        remove_block(PREV_BLKP(bp));
        remove_block(NEXT_BLKP(bp));
        remove_block(bp);
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        insert_block(PREV_BLKP(bp));
        bp = PREV_BLKP(bp);
    }
    return bp;
}

/* 
 * place - Place the requested block on heap 
 */
static void *place(void *bp, size_t asize)
{
    size_t csize = SIZE(bp);
    void *next;
    remove_block(bp);
    if ((csize - asize) >= 2 * DSIZE)
    {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        next = NEXT_BLKP(bp);
        PUT(HDRP(next), PACK(csize - asize, 0));
        PUT(FTRP(next), PACK(csize - asize, 0));
        insert_block(next);
    }
    else
    {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
    return bp;
}

/* 
 * find_fit - use first fit method to find a block that can hold the newly allocated block 
 */
static void *find_fit(size_t asize)
{
    void *bp;
    int i;
    i = getindex(asize);
    for (; i <= 15; ++i)
    {
        /* no free block*/
        if ((void *)list_head[i] == NULL)
            continue;
        bp = (char *)list_head[i];
        while (1)
        {
            /* find a suitable one*/
            if (SIZE(bp) >= asize)
            {
                return bp;
            }
            /* meet the tail*/
            if (bp == (void *)list_tail[i])
                break;
            bp = SUCC(bp); /* get to the next free block*/
        }
    }
    return NULL;
}

/* 
 * insert_block - insert newly freed block to free list
 * use the address to determine the order
 * very similar to what we do in double list 
 */
static inline void *insert_block(void *bp)
{
    /* determine which list to go*/
    int i = getindex(SIZE(bp));
    char *temp;
    if ((void *)list_head[i] == NULL)
        list_head[i] = list_tail[i] = (size_t)bp; //set up
    else
    {
        if ((size_t)(bp) < list_head[i])
        {
            PUT_SUCC(bp, list_head[i]);
            PUT_PRED(list_head[i], bp);
            list_head[i] = (size_t)bp;
        }
        else if ((size_t)(bp) > list_tail[i])
        {
            PUT_PRED(bp, list_tail[i]);
            PUT_SUCC(list_tail[i], bp);
            list_tail[i] = (size_t)bp;
        }
        else
        {
            temp = (char *)list_head[i];
            while (SUCC(temp) < (char *)bp)
                temp = SUCC(temp);
            PUT_PRED(bp, temp);
            PUT_SUCC(bp, SUCC(temp));
            PUT_PRED(SUCC(temp), bp);
            PUT_SUCC(temp, bp);
        }
    }
    return bp;
}
/* 
 *  remove_block - delete a newly allocated block in free list
 *  nearly the same as insertion 
 */
static inline void remove_block(void *bp)
{

    int i;
    i = getindex(SIZE(bp));

    if (list_head[i] == list_tail[i])
    {
        list_head[i] = list_tail[i] = (size_t)NULL;
    }
    else if ((size_t)(bp) == list_head[i])
        list_head[i] = (size_t)SUCC(bp);
    else if ((size_t)(bp) == list_tail[i])
        list_tail[i] = (size_t)PRED(bp);
    else
    {
        PUT_PRED(SUCC(bp), PRED(bp));
        PUT_SUCC(PRED(bp), SUCC(bp));
    }
}

static inline int getindex(size_t size)
{
    if (size <= SIZE1)
    {
        return 0;
    }
    else if (size <= SIZE2)
    {
        return 1;
    }
    else if (size <= SIZE3)
    {
        return 2;
    }
    else if (size <= SIZE4)
    {
        return 3;
    }
    else if (size <= SIZE5)
    {
        return 4;
    }
    else if (size <= SIZE6)
    {
        return 5;
    }
    else if (size <= SIZE7)
    {
        return 6;
    }
    else if (size <= SIZE8)
    {
        return 7;
    }
    else if (size <= SIZE9)
    {
        return 8;
    }
    else if (size <= SIZE10)
    {
        return 9;
    }
    else if (size <= SIZE11)
    {
        return 10;
    }
    else if (size <= SIZE12)
    {
        return 11;
    }
    else if (size <= SIZE13)
    {
        return 12;
    }
    else if (size <= SIZE14)
    {
        return 13;
    }
    else if (size <= SIZE15)
    {
        return 14;
    }
    else
    {
        return 15;
    }
}

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 *	here, if we find that the last block is empty, we can cut the space we are to apply
 */
static void *extend_heap(size_t asize)
{
    char *bp, *end = (char *)mem_heap_hi() - 3;
    size_t nowsize;
    if (!GET_ALLOC(end))
    {
        nowsize = asize - GET_SIZE(end);
        if ((bp = mem_sbrk(nowsize)) == (void *)-1)
            return NULL;
        bp = end - GET_SIZE(end) + DSIZE;
        remove_block(bp);
    }
    else
    {
        if ((bp = mem_sbrk(asize)) == (void *)-1)
            return NULL;
    }

    PUT(HDRP(bp), PACK(asize, 0));
    PUT(FTRP(bp), PACK(asize, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    return insert_block(bp);
}

void mm_checkheap(int lineno)
{
    char *bp = heap_listp;
    printf("heap (%p):\n", bp);
    if ((GET_SIZE(HDRP(heap_listp)) != DSIZE) || (!GET_ALLOC(HDRP(heap_listp))))
    {
        printf("prologue header error\n");
    }
    checkblock(bp);
    int prev_free = 0;
    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
    {
        printblock(bp);
        int curr_free = checkblock(bp);
        if (prev_free && curr_free)
        {
            printf("prev_free and curr_free\n");
        }
    }
    int size = 1;
    for (int i = 0; i < 16; i++)
    {
        printlist(list_head[i], size);
        checklist(list_head[i], size);
        size <<= 1;
    }
    printblock(bp);
    if ((GET_SIZE(HDRP(bp)) != 0) || !(GET_ALLOC(HDRP(bp))))
    {
        printf("prologue header error\n");
    }
    lineno = lineno;
}
static void printblock(void *bp)
{
    long hdsize, ftsize, hdalloc, ftalloc;
    hdsize = GET_SIZE(HDRP(bp));
    ftsize = GET_SIZE(FTRP(bp));
    hdalloc = GET_ALLOC(HDRP(bp));
    ftalloc = GET_ALLOC(HDRP(bp));

    if (hdsize == 0)
    {
        printf("%p:end\n");
        return;
    }
    printf("%p: header:[%ld:%c] , footer:[%ld:%c]\n",
           bp,
           hdsize, (hdalloc ? 'a' : 'f'),
           ftsize, (ftalloc ? 'a' : 'f'));
}

static int checkblock(void *bp)
{
    if ((size_t)bp % 8)
    {
        printf("%p,not aligned\n", bp);
    }
    if (GET(HDRP(bp)) != GET(FTRP(bp)))
    {
        printf("header footer not matched\n");
    }
    size_t size = GET_SIZE(HDRP(bp));
    if (size % 8)
    {
        printf("%p size not aligned (DWORD)\n");
    }
    return GET_ALLOC(HDRP(bp));
}

static void checklist(void *i, size_t size)
{
    void *prev = NULL;
    long hdsize, hdalloc;
    for (; i != NULL; i = succ(i))
    {
        if (PRED(i) != prev)
        {
            printf("prev ptr error\n");
        }
        if (prev != NULL && succ(prev) != i)
        {
            printf("succ ptr error\n");
        }
        hdsize = GET_SIZE(HDRP(i));
        hdalloc = GET_ALLOC(HDRP(i));
        if (hdalloc)
        {
            printf("list node should be free\n");
        }
        if (prev != NULL && GET_SIZE(HDRP(prev)) > hdsize)
        {
            printf("list size order error\n");
        }
        if (hdsize < size || ((size != (1 << 15)) && (hdsize > (size << 1) - 1)))
        {
            printf("list node size error\n");
        }
        prev = i;
    }
}
static void printlist(void *i, long size)
{
    long hdsize, hdalloc;
    for (; i != NULL; i = succ(i))
    {
        hdsize = GET_SIZE(HDRP(i));
        hdalloc = GET_ALLOC(HDRP(i));
        printf("[listnode %ld] %p: header:[%ld,%c] , prev:[%p] , next:[%p]\n",
               size, i,
               hdsize, (hdalloc ? 'a' : 'f'), PRED(i), SUCC(i));
    }
}
