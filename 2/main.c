#include <stdio.h>
#include <mpi/mpi.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    int npes, myrank;
    int in = 1;
    int roll = 1;

    MPI_Init (&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    int onc = npes;

    char *buf = calloc(npes, 1);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0)
        printf("536388\n");

    // pre determinizmus
    //srand(myrank);

    // pre nahodnost
    srand(myrank * time(NULL));

    MPI_Barrier(MPI_COMM_WORLD);

    while (onc != 1) {
        if (in)
            roll = rand() % 2;
        onc = npes;
        MPI_Allreduce(&roll, &onc, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        in = in && (roll || !onc);
        MPI_Barrier(MPI_COMM_WORLD);
    }
    int overlord = 0;
    MPI_Status st;
    if (myrank != 0 || !in) {
        if (in) {
            MPI_Send(&myrank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else if (myrank == 0) {
            MPI_Recv(&overlord, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
        }
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&overlord, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Sloužím ti, můj vládče, slunce naše jasné. %d\n", overlord);

    MPI_Finalize();

    free(buf);
    return 0;
}
