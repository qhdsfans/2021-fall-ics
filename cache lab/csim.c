#include "cachelab.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <string.h>

//Xtuz,2000011068,lian

typedef struct
{
    long valid;
    long tag;
    long LRUcount;
} cache_line, *cache_set, **mycache; //cache模拟,由合法位、标记位和lru数值组成，利用数组模拟整个缓存

mycache cache_0 = NULL; //初始化一个新的cache

long h, v, s, E, b, S; //参数
long hits, misses, evictions;
char tracefile[1145];//用来存放文件名

void updateLRU(long set, long location)
{
    cache_0[set][location].LRUcount = INT_MAX;//最新更新的设置为最大，其余减小
    for (long i = 0; i < E; i++)
    {
        if (i == location)
            continue;
        if (cache_0[set][i].valid == 1)
            cache_0[set][i].LRUcount--;
    }
}

long miss(long set, long tag)//用来检查是否发生miss
{
    long ismiss = 1;
    for (long i = 0; i < E; i++)
    {
        if (cache_0[set][i].valid && cache_0[set][i].tag == tag)
        {
            ismiss = 0;
            updateLRU(set, i);
        }
    }
    return ismiss;
}

long findMinLRU(long set)//用来寻找lru替换的对象
{
    long minIndex = 0;
    long minLRU = INT_MAX;
    for (long i = 0; i < E; i++)
    {
        if (cache_0[set][i].LRUcount <= minLRU)
        {
            minLRU = cache_0[set][i].LRUcount;
            minIndex = i;
        }
    }
    return minIndex;
}

long update_cache(long set, long tag)//更新缓存，同时判断缓存是否是满的，如果满了，会发生替换
{
    long isfull = 1;
    long i;
    for (i = 0; i < E; i++)
    {
        if (cache_0[set][i].valid == 0)
        {
            isfull = 0;
            break;
        }
    }
    if (!isfull)
    {
        cache_0[set][i].valid = 1;
        cache_0[set][i].tag = tag;
        updateLRU(set, i);
    }
    else
    {
        long evictionsNumber = findMinLRU(set);
        cache_0[set][evictionsNumber].valid = 1;
        cache_0[set][evictionsNumber].tag = tag;
        updateLRU(set, evictionsNumber);
    }
    return isfull;
}

void load(long addr, long set, long tag)
{
    if (miss(set, tag))
    {
        misses++;
        if (v)
            printf("miss ");
        if (update_cache(set, tag))//满了就发生替换
        {
            evictions++;
            if (v)
                printf("eviction ");
        }
    }
    else//命中，无事发生
    {
        hits++;
        if (v)
            printf("hit ");
    }
}

void init_cache()
{
    cache_0 = (mycache)malloc(sizeof(cache_set) * S);//利用malloc函数分配空间
    for (long i = 0; i < S; i++)
    {
        cache_0[i] = (cache_set)malloc(sizeof(cache_line) * E);
        for (long j = 0; j < E; j++)
        {
            cache_0[i][j].valid = 0;
            cache_0[i][j].tag = -1;
            cache_0[i][j].LRUcount = -1;
        }
    }
}

void helper()
{
    printf(
        "The reference simulator takes the following command-line arguments:\n"
        "Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
        "-h: Optional help flag that prints usage info\n"
        "-v: Optional verbose flag that displays trace info\n"
        "-s <s>: Number of set index bits (S = 2s is the number of sets)\n"
        "-E <E>: Associativity (number of lines per set)\n"
        "-b <b>: Number of block bits (B = 2b is the block size)\n"
        "-t <tracefile>: Name of the valgrind trace to rep\n");
    return;
}

long get_opt(int argc, char **argv)
{
    long c;
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1)
    {
        switch (c)
        {
        case 'v':
            v = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(tracefile, optarg);
            break;
        case 'h':
        default:
            helper();
            exit(0);
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    h = v = 0;
    hits = misses = evictions = 0;
    get_opt(argc, argv);
    if (s <= 0 || E <= 0 || b <= 0)
        return -1; //invalid args
    S = 1 << s;
    FILE *file = fopen(tracefile, "r");
    init_cache();

    long addr, size;
    char opt;
    while (fscanf(file, "%c %lx,%ld", &opt, &addr, &size) > 0)
    {
        long _set = (addr >> b) & ((1u << s) - 1);
        long _tag = addr >> (s + b);
        if (v)
            printf("%c %lx,%ld ", opt, addr, size);
        switch (opt)
        {
        case 'L':
            load(addr, _set, _tag);
            break;
        case 'M':
            load(addr, _set, _tag);//注意到M需要读一次写一次，所以就不写那个break了
        case 'S':
            load(addr, _set, _tag);
        }
        if (v)
            printf("\n");//不知道怎么回事，最后-v的输出有点问题，好像会多打印一行。。。
    }
    fclose(file);
    for (long i = 0; i < S; i++)
        free(cache_0[i]);
    free(cache_0);
    printSummary(hits, misses, evictions);
    return 0;
}
