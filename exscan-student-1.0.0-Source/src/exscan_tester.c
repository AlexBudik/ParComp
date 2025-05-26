/* @author:	Nikolaus Funk, S. Hunold
 * @date:	13.04.23
 * @info:	Test file for the exscan exercises
 */

#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#include "exscan.h"

// heavy-weight comparison against result computed by recursive inclusive scan
void scan(int A[], int n) {
  if (n == 1) return;
  
  int *B = malloc((n>>1) * sizeof(int));
  int i;
  
  for (i = 0; i < (n>>1); i++)
    B[i] = A[i<<1] + A[(i<<1) + 1];
  
  scan(B, n>>1);
  
  A[1] = B[0];
  
  for (i = 1; i < (n>>1); i++) {
    A[i<<1] += B[i-1];
    A[(i<<1) + 1] = B[i];
  }
  
  if (n & 1) A[n - 1] = B[(n>>1) - 1] + A[n - 1];
  
  free(B);
}

void usage(const char *prog_name)
{
  fprintf(stderr, "usage: %s -n <int> -e <int> [-p <int>] [-c] -h\n"
	  "-n number elements in first array\n"
	  "-e exscan version (1,2,3)\n"
	  "-p number threads\n"
	  "-c check results\n"
	  "-h print this help message\n"
	  , prog_name);
  exit(EXIT_FAILURE);
}

void fill_array(int *a, int n) {
  srand(0);
  for (int i = 0; i < n; i++) {
    a[i] = rand() & 31;
  }
}

// Test Function for the Exscan Implementations
int main(int argc, char * argv[]) {
  int i;
  int *A, *Q;
  double time;
  
  int tok;
  int n = 1, nset=0;
  int help = 0;
  int check = 0;
  int e = 0;
  int p = 1;
  int verbose = 0;
  void (*func)(int a[], int n);
  
  opterr = 0;
  
  while ((tok = getopt(argc, argv, "n:e:p:r:vhc")) != -1) {
    switch (tok) {
    case 'n':
      n = atol(optarg);
      nset = 1;
      break;
    case 'e':
      e = atoi(optarg);
      break;
    case 'p':
      p = atoi(optarg);
      omp_set_num_threads(p);
      break;
    case 'h':
      help = 1;
      break;
    case 'c':
      check = 1;
      break;
    case 'v':
      verbose = 1;
      break;
    default:
      fprintf(stderr, "unknown parameter\n");
      exit(1);
    }
  }
  
  if( (nset < 1) || help == 1 ) {
    usage(basename(argv[0]));
    exit(1);
  }
  
  // Set up test arrays
  A = malloc(n * sizeof(int));
  fill_array(A, n);
  
  if(verbose) {
    printf("input: ");
    for(int i=0; i<n; i++) {
      printf("%d ", A[i]);
    }
    printf("\n");
  }
  
  if (e == 1) {
    func = &exscan1;
  } else if(e==2) {
    func = &exscan2;
  } else if(e==3) {
    func = &exscan3;	
  } else {
    usage(basename(argv[0]));
    exit(1);
  }
  
  time = omp_get_wtime();
  func(A, n);
  time = omp_get_wtime() - time;
  printf("         n; p;  time\n");
  printf("%10d;%2d;%10.6f\n", n, p, time);
  
  if(verbose) {
    printf("result: ");
    for(int i=0; i<n; i++) {
      printf("%d ", A[i]);
    }
    printf("\n");
  }
  
  
  if( check ) {
    Q = malloc(n * sizeof(int));
    fill_array(Q, n);
    scan(Q, n);
    for (i = n-1; i > 0; i--) {
      Q[i] = Q[i - 1];
    }
    
    int ok = 1;
    for (i = 1; i < n; i++) {
      if (A[i] != Q[i]) {
	printf("Test error at element %d: %d should be %d\n", i, A[i], Q[i]);
	ok = 0;
	break;
      }
    }
    if( ok ) {
      printf("PASSED\n");
    }
    free(Q);
  }
  
  free(A);
  
  return 0;
}
