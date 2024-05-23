#include <cstdint>
#include <stdio.h>
#include <mpi/mpi.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
    printf("536388\n");
    int npes, myrank;
    char out = 0;

    MPI_Init (&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &npes);
    int onc = npes;

    char *buf = calloc(npes, 1);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    // pre determinizmus
    srand(myrank);

    // pre nahodnost
    // srand(time(NULL));

    MPI_Barrier(MPI_COMM_WORLD);

    while (onc > 1) {
        onc = npes;
        if (!out)
            out = rand() % 2;
        MPI_Allgather(&out, 1, MPI_CHAR, buf, 1, MPI_CHAR, MPI_COMM_WORLD);
        for (int i = 0; i < npes; ++i) {
            if (buf[i])
                onc--;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    int overlord = 0;
    MPI_Status st;
    if (myrank != 0 || out) {
        if (!out) {
            MPI_Send(&myrank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else if (myrank == 0) {
            MPI_Recv(&overlord, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &st);
        }
    }
    MPI_Scatter(&overlord, 1, MPI_INT, &overlord, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (overlord != myrank)
        printf("Sloužím ti, můj vládče, slunce naše jasné. %d\n", overlord);

    MPI_Finalize();

    free(buf);
    return 0;
}
