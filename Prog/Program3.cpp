#include <iostream>
#include <pthread.h>
using namespace std;

#define MAX 16
#define MAX_THREAD 4
int part = 0;
int arr[MAX] = {1,3,5,7,9,2,4,6,8,10,11,13,15,12,14,18};
int sum[2] = {};

void* array_sum(void* array)
{
    int thread_part = part++;
    for (int i = thread_part*(MAX/MAX_THREAD); i < (thread_part+1)*(MAX/MAX_THREAD); i++)
    {
        sum[thread_part] += arr[i];
    }

    pthread_exit(NULL);
    
}

int main()
{
    pthread_t threads[MAX_THREAD];

    // creating threads
    for (int i = 0; i < MAX_THREAD; i++)
    {
        pthread_create(&threads[i], NULL, array_sum, (void*)NULL);
    }

    // joining the threads after they have completed their actions
    for (int i = 0; i < MAX_THREAD; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    int sum_elements = 0;
    for (int i = 0; i < MAX_THREAD; i++)
    {
        sum_elements += sum[i];
    }
    
    int normal_sum = 0;
    for(int i = 0; i<MAX; i++)
    {
        normal_sum+= arr[i];
    }

    cout<<"Normal sum is "<< normal_sum<<endl;
    cout<<"The sum is "<<sum_elements<<endl;
    return 0;
}