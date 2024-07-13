#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Function to compute local sum
int compute_local_sum(int *local_array, int local_n) {
    int local_sum = 0;
    for (int i = 0; i < local_n; i++) {
        local_sum += local_array[i];
    }
    return local_sum;
}

int main(int argc, char *argv[]) {
    int rank, size;
    int size_of_array = 16;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Determine the local number of elements each process will handle
    int local_n = size_of_array / size;
    if (size_of_array % size != 0) {
        if (rank < size_of_array % size) {
            local_n++;
        }
    }

    // Allocate space for local part of the array
    int *local_array = (int *)malloc(local_n * sizeof(int));

    // Initialize local part of the array
    int start = rank * (size_of_array / size) + (rank < size_of_array % size ? rank : size_of_array % size);
    for (int i = 0; i < local_n; i++) {
        local_array[i] = start + i;
    }

    // Compute local sum
    int local_sum = compute_local_sum(local_array, local_n);

    // Perform tree-based reduction
    int step = 1;
    while (step < size) {
        if (rank % (2 * step) == 0) {
            if (rank + step < size) {
                int received_sum;
                MPI_Recv(&received_sum, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_sum += received_sum;
            }
        } else {
            int target = rank - step;
            MPI_Send(&local_sum, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
            break;
        }
        step *= 2;
    }

    // Process 0 prints the global sum
    if (rank == 0) {
        printf("The global sum is: %d\n", local_sum);
    }

    // Free allocated memory
    free(local_array);

    MPI_Finalize();
    return 0;
}
