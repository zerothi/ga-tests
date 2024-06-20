program pure

   use pure_m

   implicit none

   integer, parameter :: N = 500
   real, dimension(N,N) :: A, B, C

   A = 1.0
   B = 2.0

#ifdef PURE__BLAS
   call sgemm("N", "N", N, N, N, 1., A, N, B, N, 0., C, N)
   print *, "sgemm", sum(C)
#endif

   call simple(A, B, C)
   print *, "simple", sum(C)

end program
