#!../c
program mandelbrot
  implicit none

  Integer, parameter :: grid = 10, iter = 90

  Complex :: c
  Integer :: i, j
  real :: xpart, ypart
  Logical :: converges


  do i = 0, grid
  do j = 0, grid
  xpart = -2.0 + 4.0*real(i)/real(grid)
  ypart = -2.0 + 4.0*real(j)/real(grid)
  c = cmplx(xpart, ypart)

  if (converges(c, iter)) then
    print *, c
  end if

  end do
  end do
end program mandelbrot


function converges (c, iter) result (convergence)
  implicit none
  Complex, intent(in) :: c
  Integer, intent(in) :: iter

  Real, parameter :: threshold = 2.0
  Integer :: i
  Complex :: buffer
  Logical :: convergence
  buffer = (0,0)

  do i=0, iter
  buffer = buffer**2 + c
  if (abs(buffer)>threshold) then
    convergence = .false.
    return
  end if
  end do
  convergence = .true.
end function
