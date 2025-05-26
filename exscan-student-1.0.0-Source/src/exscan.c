/* @author:	Nikolaus Funk
 * @date:	13.04.23
 * @info:	Implement your exscan algorithms here
 */

#include<stdio.h>
#include<omp.h>
#include<stdlib.h>


void exscan1(int A[], int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        int temp = A[i];
        A[i] = sum;
        sum += temp;
    }
}

void up_sweep(int A[], int n) {
    for (int step = 1; step < n; step *= 2) {
        for (int i = (n / (2 * step)) - 1; i >= 0; i--) {
            int index = (2 * i + 1) * step - 1;
            A[index + step] += A[index];
        }
    }
}

void down_sweep(int A[], int n) {
    A[n - 1] = 0;

    for (int step = n / 2; step >= 1; step /= 2) {
        for (int i = 0; i < n / (2 * step); i++) {
            int left = (2 * i + 1) * step - 1;
            int right = left + step;

            int temp = A[left];
            A[left] = A[right];
            A[right] += temp;
        }
    }
}

void exscan2(int A[], int n) {
    if (n == 0) return;

    int pow2 = 1;
    while (pow2 < n) pow2 *= 2;

    int *B = calloc(pow2, sizeof(int));
    for (int i = 0; i < n; i++) {
        B[i] = A[i];
    }

    up_sweep(B, pow2);
    down_sweep(B, pow2);

    for (int i = 0; i < n; i++) {
        A[i] = B[i];
    }

    free(B);
}

void up_sweep_parallel(int A[], int n) {
    for (int step = 1; step < n; step *= 2) {
        #pragma omp parallel for
        for (int i = 0; i < n / (2 * step); i++) {
            int index = (2 * i + 1) * step - 1;
            A[index + step] += A[index];
        }
    }
}

void down_sweep_parallel(int A[], int n) {
    A[n - 1] = 0;

    for (int step = n / 2; step >= 1; step /= 2) {
        #pragma omp parallel for
        for (int i = 0; i < n / (2 * step); i++) {
            int left = (2 * i + 1) * step - 1;
            int right = left + step;

            int temp = A[left];
            A[left] = A[right];
            A[right] += temp;
        }
    }
}

void exscan3(int A[], int n) {
    if (n == 0) return;

    int pow2 = 1;
    while (pow2 < n) pow2 *= 2;

    int *B = calloc(pow2, sizeof(int));

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        B[i] = A[i];
    }

    up_sweep_parallel(B, pow2);
    down_sweep_parallel(B, pow2);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        A[i] = B[i];
    }

    free(B);
}