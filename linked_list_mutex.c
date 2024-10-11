#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "linked_list.h"

int num_threads_mutex;
int num_operations_mutex;
int memberCountMutex = 0;
int insertCountMutex = 0;
int deleteCountMutex = 0;

pthread_mutex_t mutex;

struct list_node_s *head_mutex = NULL;

struct thread_args
{
    int id;
    double mMember;
    double mInsert;
    double mDelete;
};

void *mutexOperation(void *args)
{
    struct thread_args *args_p = (struct thread_args *)args;
    int m = num_operations_mutex;
    int id = args_p->id;
    double mMember = args_p->mMember;
    double mInsert = args_p->mInsert;
    double mDelete = args_p->mDelete;

    for (int i = id; i < num_operations_mutex;)
    {
        double opType = (double)rand() / RAND_MAX;
        int value = rand() % (1 << 16);

        if ((opType < mMember) && (memberCountMutex < (m * mMember)))
        {
            pthread_mutex_lock(&mutex);
            Member(value, head_mutex);
            memberCountMutex++;
            i += num_threads_mutex;
            pthread_mutex_unlock(&mutex);
        }
        else if ((opType < mMember + mInsert) && (insertCountMutex < (m * mInsert)))
        {
            pthread_mutex_lock(&mutex);
            Insert(value, &head_mutex);
            insertCountMutex++;
            i += num_threads_mutex;
            pthread_mutex_unlock(&mutex);
        }
        else if (deleteCountMutex < (m * mDelete))
        {
            pthread_mutex_lock(&mutex);
            Delete(value, &head_mutex);
            deleteCountMutex++;
            i += num_threads_mutex;
            pthread_mutex_unlock(&mutex);
        }
    }
}

double performMutexOperationOnLinkedList(double mMember, double mInsert, double mDelete, int numThreads)
{
    num_threads_mutex = numThreads;
    num_operations_mutex = 10000;
    PopulateLinkedList(1000, &head_mutex);

    pthread_t threads[num_threads_mutex];
    pthread_mutex_init(&mutex, NULL);

    struct thread_args *args_p = malloc(sizeof(struct thread_args));
    args_p->mMember = mMember;
    args_p->mInsert = mInsert;
    args_p->mDelete = mDelete;

    clock_t start = clock();

    for (int i = 0; i < num_threads_mutex; i++)
    {
        args_p->id = i;
        pthread_create(&threads[i], NULL, mutexOperation, (void *)args_p);
    }

    for (int i = 0; i < num_threads_mutex; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    clock_t end = clock();

    double times = ((double)(end - start)) / CLOCKS_PER_SEC;

    FreeLinkedList(&head_mutex);
    memberCountMutex = 0;
    insertCountMutex = 0;
    deleteCountMutex = 0;

    return times;
}
