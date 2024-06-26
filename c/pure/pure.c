#include <stdio.h>
#include <stdlib.h>

#include "pure_lib.h"

void initialize(const int N, float *M, const float val) {
   for (int i = 0 ; i < N ; i++ ) {
      M[i] = val;
   }
}

float sum(const int N, float *M) {
   float sum;
   for (int i = 0 ; i < N ; i++ ) {
      sum += M[i];
   }
   return sum;
}


int main(int nargs, char* args[]) {

   static int N = 500;

   float *A, *B, *C;

   A = malloc(sizeof(float) * N * N);
   B = malloc(sizeof(float) * N * N);
   C = malloc(sizeof(float) * N * N);

   initialize(N * N, A, 1.f);
   initialize(N * N, B, 2.f);
   initialize(N * N, C, 0.f);

#ifdef PURE__BLAS
   sgemm_("N", "N", N, N, N, 1.f, A, N, B, N, 0.f, C, N);
   printf("sgemm %f\n", sum(N*N, C));
#endif

   simple(N, A, B, C);
   printf("simple %f\n", sum(N*N, C));

   return 0;
}
