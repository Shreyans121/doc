#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREAD 5

void *printHello(void *threadid);

int main()
{
    pthread_t threads[NUM_THREAD];

    int rc = 0;

    for (long i = 0; i < NUM_THREAD; i++)
    {
        cout << "main(): creating thread: " << i << endl;
        rc = pthread_create(&threads[i], NULL, printHello, (void *)i);

        if (rc)
        {
            cout << "Error: Unable to create thread" << rc << endl;
            exit(-1);
        }
    }

    pthread_exit(NULL);
}

void *printHello(void *threadid)
{
    long thread_id = (long)threadid;
    cout << "Hello World! Thread ID = " << thread_id << endl;
    pthread_exit(NULL);
}