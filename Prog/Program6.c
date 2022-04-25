#include <omp.h>
#include <stdio.h>

void reset_freq(int *freq, int THREADS)
{
    for (int i = 0; i < THREADS; i++)
    {
        freq[i] = 0;
    }
}
int main(int *argc, char **argv)
{
    int n, THREADS, i;
    printf("Enter the number of iterations :");
    scanf("%d", &n);
    printf("Enter the number of threads (max 8): ");
    scanf("%d", &THREADS);
    int freq[6];
    reset_freq(freq, THREADS);
    // simple parallel for with unequal iterations
#pragma omp parallel for num_threads(THREADS)
    for (i = 0; i < n; i++)
    {
        freq[omp_get_thread_num()]++;
    }
#pragma omp barrier
    printf("\nIn default scheduling, we have the following thread distribution :-\n");
    for (int i = 0; i < THREADS; i++)
    {
        printf("Thread No. %d : %d iterations\n", i, freq[i]);
    }
    // using static scheduling
    int CHUNK;
    printf("\nUsing static scheduling...\n");
    printf("Enter the chunk size :");
    scanf("%d", &CHUNK);

    reset_freq(freq, THREADS);
#pragma omp parallel for num_threads(THREADS) schedule(static, CHUNK)
    for (i = 0; i < n; i++)
    {
        freq[omp_get_thread_num()]++;
    }
#pragma omp barrier
    printf("\nIn static scheduling, we have the following thread distribution :- \n");
    for (int i = 0; i < THREADS; i++)
    {
        printf("Thread No. %d : %d iterations\n", i, freq[i]);
    }
    // auto scheduling depending on the compiler
    printf("\nUsing automatic scheduling...\n");
    reset_freq(freq, THREADS);
#pragma omp parallel for num_threads(THREADS) schedule(static)
    for (i = 0; i < n; i++)
    {
        freq[omp_get_thread_num()]++;
    }
#pragma omp barrier
    printf("In auto scheduling, we have the following thread distribution :-\n");
    for (int i = 0; i < THREADS; i++)
    {
        printf("Thread No. %d : %d iterations\n", i, freq[i]);
    }
    return 0;
}