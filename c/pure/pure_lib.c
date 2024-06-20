
void simple(const int N,
      float *restrict A,
      float *restrict B,
      float *restrict C) {

   int i,j,l;

   for( int i = 0 ; i < N ; i++) {
      for( int j = 0 ; j < N ; j++) {
         for( int l = 0 ; l < N ; l++) {
            C[l*N + j] += A[i*N + j] * B[j*N + l];
         }
      }
   }
}
