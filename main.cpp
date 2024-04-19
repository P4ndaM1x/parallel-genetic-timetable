// MPI program for calculating Pi by numerical integration - basic code framework

#include "mpi.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    int n = 0, myid = 0, numprocs = 1, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    /***
     * Calculate pi by numerical integration of a arctangent derivative function.
     * Read a number of stripes (rectangles or trapezes) under
     * the function curve in every loop iteration
     */

    while (true) {
        if (myid == 0) {
            printf("Enter the number of intervals: (0 quits) ");
            scanf("%d", &n);
        }
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (n == 0)
            break;

        h = 1.0 / (double)n;
        sum = 0.0;
        for (i = myid + 1; i <= n; i += numprocs) {
            x = h * ((double)i - 0.5);
            sum += 4.0 / (1.0 + x * x);
        }
        mypi = h * sum;

        // pi = mypi; /* !!! seriously? serially?  */
        MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (myid == 0)
            printf("pi is approximately %.16f, Error is %.16f\n",
                pi, fabs(pi - PI25DT));
    }

    MPI_Finalize();
    return 0;
}
