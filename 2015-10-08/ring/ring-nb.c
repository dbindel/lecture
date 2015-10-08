/*
 * Ring example using nonblocking communication
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


void interact(double* result, double x, double y)
{
    *result += fabs(x-y);
}


void self_interacts(double* result, double* x, int n)
{
    int i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            if (i != j)
                interact(result+i, x[i], x[j]);    
}


void interacts(double* result, double* x, double* y, int n)
{
    int i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            interact(result+i, x[i], y[j]);    
}


typedef struct exchange_t {
    double* curr_buf;
    double* send_buf;
    double* recv_buf;
    int nper, next, prev;
    MPI_Request requests[2];
    MPI_Status  statuses[2];
} exchange_t;


void init_exchange(exchange_t* xdata, int nper)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    xdata->next = (rank+1) % size;
    xdata->prev = (rank+size-1) % size;
    xdata->nper = nper;
    xdata->curr_buf = (double*) malloc(nper * sizeof(double));
    xdata->send_buf = (double*) malloc(nper * sizeof(double));
    xdata->recv_buf = (double*) malloc(nper * sizeof(double));
}


void free_exchange(exchange_t* xdata)
{
    free(xdata->curr_buf);
    free(xdata->send_buf);
    free(xdata->recv_buf);
}


void start_exchange(exchange_t* xdata, int tag)
{
    memcpy(xdata->send_buf, xdata->curr_buf, xdata->nper * sizeof(double));
    MPI_Isend(xdata->send_buf, xdata->nper, MPI_DOUBLE, xdata->next, tag,
              MPI_COMM_WORLD, xdata->requests+0);
    MPI_Irecv(xdata->recv_buf, xdata->nper, MPI_DOUBLE, xdata->prev, tag,
              MPI_COMM_WORLD, xdata->requests+1);
}


void finish_exchange(exchange_t* xdata)
{
    double* tmp;
    MPI_Waitall(2, xdata->requests, xdata->statuses);
    tmp = xdata->curr_buf;
    xdata->curr_buf = xdata->recv_buf;
    xdata->recv_buf = tmp;
}


int main(int argc, char** argv)
{
    int nper = 2;
    int i, phase;
    int size, rank;
    
    double* result   = (double*) malloc(nper * sizeof(double));
    double* my_data  = (double*) malloc(nper * sizeof(double));
    exchange_t xdata;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    init_exchange(&xdata, nper);
    
    /* Populate local arrays */
    memset(result, 0, nper * sizeof(double));
    for (i = 0; i < nper; ++i)
        my_data[i] = rank + 0.1 * i;
    memcpy(xdata.curr_buf, my_data, nper * sizeof(double));

    if (size > 1)
        start_exchange(&xdata, 0);
    self_interacts(result, my_data, nper);

    for (phase = 0; phase < size-1; ++phase) {
        
        finish_exchange(&xdata);
        if (phase < size-2)
            start_exchange(&xdata, 0);
        interacts(result, my_data, xdata.curr_buf, nper);
        
        if (rank == 0) {
            printf("Interact %g-%g vs %g-%g\n",
                   my_data[0], my_data[1],
                   xdata.curr_buf[0], xdata.curr_buf[nper-1]);
        }
        
    }

    free_exchange(&xdata);
    free(my_data);
    free(result);

    MPI_Finalize();
    return 0;
}
