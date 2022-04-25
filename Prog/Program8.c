#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
int thread_count;
// gcc name_of_file.c -o name_of_exe -lpthread (link p thread)
// necessary for referencing in the thread
struct arguments
{
    int size;
    int *arr1;
    int *arr2;
    int *dot;
};

// function to parallelize`
void *add_into_one(void *arguments);
// util
void print_vector(int n, int *arr)
{
    printf("[ ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("] \n");
}
// main driver function of the program
int main(int argc, char *argv[])
{
    long thread;
    pthread_t *thread_handles;
    thread_count = 2; // using 2 threads only
    // get the thread handles equal to total num
    thread_handles = (pthread_t *)malloc(thread_count * sizeof(pthread_t));
    printf("Enter the vector size: ");
    int n;
    scanf("%d", &n);
    printf("Enter vectors maximal element: ");
    int max_val;
    scanf("%d", &max_val);
    struct arguments *args[2]; // array of pointer to structure
    for (int i = 0; i < 2; i++)
    {
        // allocate for the struct
        args[i] = (struct arguments *)malloc(sizeof(struct arguments) * 1);
        // allocate for the arrays
        args[i]->size = n;
        args[i]->arr1 = (int *)malloc(sizeof(int) * n);
        args[i]->arr2 = (int *)malloc(sizeof(int) * n);
        args[i]->dot = (int *)malloc(sizeof(int) * n);
        for (int j = 0; j < n; j++)
        {
            args[i]->arr1[j] = rand() % max_val;
            args[i]->arr2[j] = rand() % max_val;
        }
    }
    printf("Vectors are : \n");
    print_vector(n, args[0]->arr1);
    print_vector(n, args[0]->arr2);
    print_vector(n, args[1]->arr1);
    print_vector(n, args[1]->arr2);
    int result[n];

    memset(result, 0, n * sizeof(int));

    // note : we need to manually startup our threads
    // for a particular function which we want to execute in
    // the thread
    for (thread = 0; thread < thread_count; thread++)
    {
        printf("Multiplying %ld and %ld with thread %ld...\n", thread + 1,
               thread + 2,
               thread);
        pthread_create(&thread_handles[thread], NULL, add_into_one,
                       (void *)args[thread]);
    }
    printf("Currently in the main thread\n");
    // wait for completion
    for (thread = 0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);
    for (int i = 0; i < 2; i++)
    {
        printf("Multiplication for vector %d and %d \n", i + 1, i + 2);
        print_vector(n, args[i]->dot);
        printf("\n");
    }
    free(thread_handles);
    // now compute the summation of results
    for (int i = 0; i < n; i++)
        result[i] = args[0]->dot[i] + args[1]->dot[i];
    printf("Result is : \n");
    print_vector(n, result);
    return 0;
}
void *add_into_one(void *argument)
{
    // de reference the argument
    struct arguments *args = (struct arguments *)argument;
    // compute the dot product into the
    // array dot
    int n = args->size;
    for (int i = 0; i < n; i++)
        args->dot[i] = args->arr1[i] * args->arr2[i];
    return NULL;
}
