#include <omp.h>
#include <stdio.h>
int main(int *argc, char **argv)
{ // invocation of the main program
    // use the fopenmp flag for compiling
    int num_threads, THREAD_COUNT = 4;
    int thread_ID;
    int section_sizes[4] = {
        0, 100, 200, 300};
    printf("Implementing work sharing of threads:\n");
#pragma omp parallel private(thread_ID) num_threads(THREAD_COUNT)
    {
        // private means each thread will have a private variable
        // thread_ID
        thread_ID = omp_get_thread_num();
        printf("Current thread number %d!\n", thread_ID);
        int value_count = 0;
        if (thread_ID > 0)
        {
            int work_load = section_sizes[thread_ID];
            // each thread has a different section size
            for (int i = 0; i < work_load; i++)
                value_count++;
            printf("Total no. of values computed are : %d\n",
                   value_count);
        }
#pragma omp barrier
        if (thread_ID == 0)
        {
            printf("Total number of threads are : %d\n",
                   omp_get_num_threads());
        }
    }
    return 0;
}