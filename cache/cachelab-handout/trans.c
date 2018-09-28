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

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int t1, t2, t3, t4;
    for (int i = 0; i < N / 8; ++i) {
        for (int j = 0; j < M / 8; ++j) {
            if (M == 32) {
                for (int r = 0; r < 8; ++r) {
                    for (int s = 0; s < 8; ++s) {
                        if (i == j && r == s) {
                            t1 = r;
                            t2 = A[i * 8 + r][j * 8 + s];
                        } else {
                            t3 = A[i * 8 + r][j * 8 + s];
                            B[j * 8 + s][i * 8 + r] = t3;
                        }
                    }
                    if (i == j) {
                        B[j * 8 + t1][i * 8 + t1] = t2;
                    }
                }
            } else {
                //#1
                for (int r = 0; r < 4; ++r) {
                    t1 = A[i * 8 + r][j * 8 + 0];
                    t2 = A[i * 8 + r][j * 8 + 1];
                    t3 = A[i * 8 + r][j * 8 + 2];
                    t4 = A[i * 8 + r][j * 8 + 3];

                    B[j * 8 + 0][i * 8 + r] = t1;
                    B[j * 8 + 1][i * 8 + r] = t2;
                    B[j * 8 + 2][i * 8 + r] = t3;
                    B[j * 8 + 3][i * 8 + r] = t4;

                }
                //#2
                for (int r = 0; r < 4; ++r) {
                    t1 = A[i * 8 + r][j * 8 + 4];
                    t2 = A[i * 8 + r][j * 8 + 5];
                    t3 = A[i * 8 + r][j * 8 + 6];
                    t4 = A[i * 8 + r][j * 8 + 7];


                    B[j * 8 + 4][i * 8 + r] = t1;
                    B[j * 8 + 5][i * 8 + r] = t2;
                    B[j * 8 + 6][i * 8 + r] = t3;
                    B[j * 8 + 7][i * 8 + r] = t4;
                }

                //#3
                for (int r = 4; r < 8; ++r) {
                    t1 = A[i * 8 + r][j * 8 + 0];
                    t2 = A[i * 8 + r][j * 8 + 1];
                    t3 = A[i * 8 + r][j * 8 + 2];
                    t4 = A[i * 8 + r][j * 8 + 3];


                    B[j * 8 + 0][i * 8 + r] = t1;
                    B[j * 8 + 1][i * 8 + r] = t2;
                    B[j * 8 + 2][i * 8 + r] = t3;
                    B[j * 8 + 3][i * 8 + r] = t4;

                }
                //#4
                for (int r = 4; r < 8; ++r) {
                    t1 = A[i * 8 + r][j * 8 + 4];
                    t2 = A[i * 8 + r][j * 8 + 5];
                    t3 = A[i * 8 + r][j * 8 + 6];
                    t4 = A[i * 8 + r][j * 8 + 7];

                    B[j * 8 + 4][i * 8 + r] = t1;
                    B[j * 8 + 5][i * 8 + r] = t2;
                    B[j * 8 + 6][i * 8 + r] = t3;
                    B[j * 8 + 7][i * 8 + r] = t4;
                }
            }
        }
    }
    for (int i = 0; i < N / 8 * 8; ++i) {
        for (int j = 0; j < M % 8; ++j) {
            B[M / 8 * 8 + j][i] = A[i][M / 8 * 8 + j];
        }
    }
    for (int j = 0; j < M; ++j) {
        for (int i = N / 8 * 8; i < N; ++i) {
            B[j][i] = A[i][j];
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

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

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

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

