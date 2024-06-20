module pure_m

   implicit none

contains

   subroutine simple(A, B, C)
      real, dimension(:, :), intent(in) :: A, B
      real, dimension(:, :), intent(out) :: C

      integer :: N, M, K, i, j, l

      N = size(A, 1)
      M = size(A, 2)
      K = size(B, 2)

      if ( M /= size(B, 1) ) then
         stop
      end if

      C(:, :) = 0.0
      do i = 1 , N
         do j = 1 , M
            do l = 1 , K
               C(l, j) = C(l, j) + A(i, j) * B(j, l)
            end do
         end do
      end do

   end subroutine

end module
