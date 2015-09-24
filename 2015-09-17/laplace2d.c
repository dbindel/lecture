#include <stdio.h>
#include <math.h>


double u(double x, double y)
{
    return cos(x) * sin(y);
}


double laplacian_u(double (*u)(double x, double y),
                   double h, double x, double y)
{
    /* Fill in the solution here */
    double upx = u(x+h,y);
    double umx = u(x-h,y);
    double upy = u(x,y+h);
    double umy = u(x,y-h);
    double u0  = u(x,y);
    return (4*u0-upx-upy-umx-umy)/(h*h);
}


int main()
{
    printf("Numerical Laplacian at (0.8,0.5) = %g\n",
           laplacian_u(u, 1e-3, 0.8, 0.5));
    printf("True Laplacian at (0.8,0.5) = %g\n",
           2 * cos(0.8) * sin(0.5));
    return 0;
}
