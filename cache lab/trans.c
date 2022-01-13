/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include "contracts.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. The REQUIRES and ENSURES from 15-122 are included
 *     for your convenience. They can be removed if you like.
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    if (M == 32)
    {
        int i, j, x;
        int v1, v2, v3, v4, v5, v6, v7, v8; //注意到一个块刚好可以放8个int，所以采取8x8分块，同时用空间换时间，一次读取一行

        for (i = 0; i < M; i += 8)
        {
            for (j = 0; j < N; j += 8)
            {
                for (x = i; x < i + 8; ++x)
                {
                    v1 = A[x][j]; //从A中读数据
                    v2 = A[x][j + 1];
                    v3 = A[x][j + 2];
                    v4 = A[x][j + 3];
                    v5 = A[x][j + 4];
                    v6 = A[x][j + 5];
                    v7 = A[x][j + 6];
                    v8 = A[x][j + 7];
                    B[j][x] = v1; //用B覆盖A占有的缓存块
                    B[j + 1][x] = v2;
                    B[j + 2][x] = v3;
                    B[j + 3][x] = v4;
                    B[j + 4][x] = v5;
                    B[j + 5][x] = v6;
                    B[j + 6][x] = v7;
                    B[j + 7][x] = v8;
                }
            }
        }
    }
    //缓存只能放四行，如果继续8*8,会造成很多冲突不命中，所以尝试4*4，结果卡在1700左右优化不下去
    //最后还是用8*8,为了充分利用可以利用的8个int变量，对8*8的块再次分割
    //部分东西先存到B里面，再调整
    if (M == 64)
    {
        int i, j, x, y;
        int v1, v2, v3, v4, v5, v6, v7, v8;
        for (i = 0; i < N; i += 8)
        {
            for (j = 0; j < M; j += 8)
            {
                for (x = i; x < i + 4; ++x) //A的左上角正常转置，右上角转过来放到B的右上角
                {
                    v1 = A[x][j];
                    v2 = A[x][j + 1];
                    v3 = A[x][j + 2];
                    v4 = A[x][j + 3];
                    v5 = A[x][j + 4];
                    v6 = A[x][j + 5];
                    v7 = A[x][j + 6];
                    v8 = A[x][j + 7];

                    B[j][x] = v1;
                    B[j + 1][x] = v2;
                    B[j + 2][x] = v3;
                    B[j + 3][x] = v4;
                    B[j][x + 4] = v5;
                    B[j + 1][x + 4] = v6;
                    B[j + 2][x + 4] = v7;
                    B[j + 3][x + 4] = v8;
                }
                for (y = j; y < j + 4; ++y) //调整右上角到B的左下角，同时用A的左下角填充B的右上角
                {
                    v1 = A[i + 4][y];
                    v2 = A[i + 5][y];
                    v3 = A[i + 6][y];
                    v4 = A[i + 7][y];
                    v5 = B[y][i + 4];
                    v6 = B[y][i + 5];
                    v7 = B[y][i + 6];
                    v8 = B[y][i + 7];

                    B[y][i + 4] = v1;
                    B[y][i + 5] = v2;
                    B[y][i + 6] = v3;
                    B[y][i + 7] = v4;
                    B[y + 4][i] = v5;
                    B[y + 4][i + 1] = v6;
                    B[y + 4][i + 2] = v7;
                    B[y + 4][i + 3] = v8;
                }
                for (x = i + 4; x < i + 8; ++x) //A的右下角进行转置
                {
                    v1 = A[x][j + 4];
                    v2 = A[x][j + 5];
                    v3 = A[x][j + 6];
                    v4 = A[x][j + 7];
                    B[j + 4][x] = v1;
                    B[j + 5][x] = v2;
                    B[j + 6][x] = v3;
                    B[j + 7][x] = v4;
                }
            }
        }
    }
    if (M == 60)
    {
        int i, j, x;
        int v1, v2, v3, v4, v5, v6, v7, v8;
        //先做一个60*60的转置，然后完成最后的60*8的转置，应该还可以继续优化但是写了很多都没有这个版本好所以就放弃了（x
        for (i = 0; i < 60; i += 4)
        {
            for (j = 0; j < 60; j += 4)
            {
                for (x = i; x < i + 4; ++x)
                {
                    v1 = A[x][j]; //从A中读数据
                    v2 = A[x][j + 1];
                    v3 = A[x][j + 2];
                    v4 = A[x][j + 3];
                    B[j][x] = v1; //用B覆盖A占有的缓存块
                    B[j + 1][x] = v2;
                    B[j + 2][x] = v3;
                    B[j + 3][x] = v4;
                }
            }
        }

        for (i = 0; i < 60; ++i)
        {
            v1 = B[i][60]; //从A中读数据
            v2 = B[i][61];
            v3 = B[i][62];
            v4 = B[i][63];
            v5 = B[i][64];
            v6 = B[i][65];
            v7 = B[i][66];
            v8 = B[i][67];
            A[60][i] = v1; //用B覆盖A占有的缓存块
            A[61][i] = v2;
            A[62][i] = v3;
            A[63][i] = v4;
            A[64][i] = v5;
            A[65][i] = v6;
            A[66][i] = v7;
            A[67][i] = v8;
        }
    }
}

/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
  * trans - A simple baseline transpose function, not optimized for the cache.
  */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    REQUIRES(M > 0);
    REQUIRES(N > 0);

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; j++)
        {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

    ENSURES(is_transpose(M, N, A, B));
}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);
}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < M; ++j)
        {
            if (A[i][j] != B[j][i])
            {
                return 0;
            }
        }
    }
    return 1;
}
