#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

#define max_count 1000
#define send_data_tag 2001
#define return_data_tag 2002

int array[max_count];
int array2[max_count];

int main(int argc, char **argv)
{
    int sum, partial_sum;
    MPI_Status status;
    int my_id, root_process, ierr, i, num_rows, num_procs,
        an_id, num_rows_to_receive, avg_rows_per_process,
        sender, num_rows_received, start_row, end_row, num_rows_to_send;

    /* Now replicte this process to create parallel processes.
     * From this point on, every process executes a seperate copy
     * of this program */

    ierr = MPI_Init(&argc, &argv);

    root_process = 0;

    /* find out MY process ID, and how many processes were started. */

    ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (my_id == root_process)
    {

        /* determine how many numbers to sum */

        printf("Please enter the number of numbers in array: \n");
        scanf("%i", &num_rows);
        printf("Number of elements in array: %d\n", num_rows);
        printf("Number of processors used: %d\n", num_procs);

        if (num_rows > max_count)
        {
            printf("Too many numbers.\n");
            exit(1);
        }

        avg_rows_per_process = num_rows / num_procs;

        /* initialize an array */

        for (i = 0; i < num_rows; i++)
        {
            array[i] = i + 1;
        }

        /* distribute arrays to each child process */

        for (an_id = 1; an_id < num_procs; an_id++)
        {
            start_row = an_id * avg_rows_per_process + 1;
            end_row = (an_id + 1) * avg_rows_per_process;

            if ((num_rows - end_row) < avg_rows_per_process)
            {
                end_row = num_rows - 1;
            }

            num_rows_to_send = end_row - start_row + 1;

            ierr = MPI_Send(&num_rows_to_send, 1, MPI_INT,
                            an_id, send_data_tag, MPI_COMM_WORLD);

            ierr = MPI_Send(&array[start_row], num_rows_to_send, MPI_INT,
                            an_id, send_data_tag, MPI_COMM_WORLD);
        }

        /* and calculate the sum of the values in the segment assigned
         * to the root process */

        sum = 0;
        for (i = 0; i < avg_rows_per_process + 1; i++)
        {
            sum += array[i];
        }

        printf("Sum %i calculated by root process\n", sum);

        /* and, finally, I collet the partial sums from the slave processes,
         * print them, and add them to the grand sum, and print it */

        for (an_id = 1; an_id < num_procs; an_id++)
        {

            ierr = MPI_Recv(&partial_sum, 1, MPI_LONG, MPI_ANY_SOURCE,
                            return_data_tag, MPI_COMM_WORLD, &status);

            sender = status.MPI_SOURCE;

            printf("Partial sum %i returned from process %i\n", partial_sum, sender);

            sum += partial_sum;
        }

        printf("Total sum is: %i\n", sum);
    }

    else
    {

        /* receive the segment, storing it in a local array, array1 */

        ierr = MPI_Recv(&num_rows_to_receive, 1, MPI_INT,
                        root_process, send_data_tag, MPI_COMM_WORLD, &status);

        ierr = MPI_Recv(&array2, num_rows_to_receive, MPI_INT,
                        root_process, send_data_tag, MPI_COMM_WORLD, &status);

        num_rows_received = num_rows_to_receive;

        /* Calculate the sum of my portion of the array */

        partial_sum = 0;
        for (i = 0; i < num_rows_received; i++)
        {
            partial_sum += array2[i];
        }

        /* send partial sum to the root process */

        ierr = MPI_Send(&partial_sum, 1, MPI_LONG, root_process,
                        return_data_tag, MPI_COMM_WORLD);
    }
    ierr = MPI_Finalize();
}