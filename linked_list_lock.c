#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#include "linked_list.h"

int num_threads_rwlock;
int num_operations_rwlock;
int memberCountRW = 0;
int insertCountRW = 0;
int deleteCountRW = 0;

pthread_rwlock_t rwlock;

struct list_node_s *head_rw = NULL;

struct thread_args
{
    int id;
    double mMember;
    double mInsert;
    double mDelete;
};

void *rwlockOperation(void *args)
{
    struct thread_args *args_p = (struct thread_args *)args;
    int m = num_operations_rwlock;
    int id = args_p->id;
    double mMember = args_p->mMember;
    double mInsert = args_p->mInsert;
    double mDelete = args_p->mDelete;

    for (int i = id; i < num_operations_rwlock;)
    {
        double opType = (double)rand() / RAND_MAX;
        int value = rand() % (1 << 16);

        if ((opType < mMember) && (memberCountRW < (m * mMember)))
        {
            pthread_rwlock_rdlock(&rwlock);
            Member(value, head_rw);
            memberCountRW++;
            i += num_threads_rwlock;
            pthread_rwlock_unlock(&rwlock);
        }
        else if ((opType < mMember + mInsert) && (insertCountRW < (m * mInsert)))
        {
            pthread_rwlock_wrlock(&rwlock);
            Insert(value, &head_rw);
            insertCountRW++;
            i += num_threads_rwlock;
            pthread_rwlock_unlock(&rwlock);
        }
        else if (deleteCountRW < (m * mDelete))
        {
            pthread_rwlock_wrlock(&rwlock);
            Delete(value, &head_rw);
            deleteCountRW++;
            i += num_threads_rwlock;
            pthread_rwlock_unlock(&rwlock);
        }
    }
}

double performLockOperationOnLinkedList(double mMember, double mInsert, double mDelete, int numThreads)
{
    num_threads_rwlock = numThreads;
    num_operations_rwlock = 10000;
    PopulateLinkedList(1000, &head_rw);

    pthread_t threads[num_threads_rwlock];
    pthread_rwlock_init(&rwlock, NULL);

    struct thread_args *args_p = malloc(sizeof(struct thread_args));
    args_p->mMember = mMember;
    args_p->mInsert = mInsert;
    args_p->mDelete = mDelete;

    clock_t start = clock();

    for (int i = 0; i < num_threads_rwlock; i++)
    {
        args_p->id = i;
        pthread_create(&threads[i], NULL, rwlockOperation, (void *)args_p);
    }

    for (int i = 0; i < num_threads_rwlock; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    clock_t end = clock();

    double times = ((double)(end - start)) / CLOCKS_PER_SEC;

    FreeLinkedList(&head_rw);
    memberCountRW = 0;
    insertCountRW = 0;
    deleteCountRW = 0;

    return times;
}
