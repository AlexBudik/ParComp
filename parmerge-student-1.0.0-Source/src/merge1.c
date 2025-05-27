/*
 * parmerge.c
 *
 *  Created on: Mar 12, 2019
 *      Author: sascha
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "merge.h"

int rank(double x, double X[], int n) {
    int low = 0, high = n;
    while (low < high) {
        int mid = (low + high) / 2;
        if (X[mid] < x) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}

void merge(double a[], long n, double b[], long m, double c[]) {
    #pragma omp parallel for
    for (long i = 0; i < n; i++) {
        int r = rank(a[i], b, m);
        c[i + r] = a[i];
    }

    #pragma omp parallel for
    for (long j = 0; j < m; j++) {
        int r = rank(b[j], a, n);
        c[j + r] = b[j];
    }
}


