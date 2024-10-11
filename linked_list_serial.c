#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "linked_list.h"

int memberCountSerial = 0;
int insertCountSerial = 0;
int deleteCountSerial = 0;

struct list_node_s *head_serial = NULL;

void serialOperation(int m, double mMember, double mInsert, double mDelete)
{

    for (int i = 0; i < m;)
    {
        double opType = (double)rand() / RAND_MAX;
        int value = rand() % (1 << 16);

        if ((opType < mMember) && (memberCountSerial < (m * mMember)))
        {
            Member(value, head_serial);
            memberCountSerial++;
            i++;
        }
        else if ((opType < mMember + mInsert) && (insertCountSerial < (m * mInsert)))
        {
            Insert(value, &head_serial);
            insertCountSerial++;
            i++;
        }
        else if (deleteCountSerial < (m * mDelete))
        {
            Delete(value, &head_serial);
            deleteCountSerial++;
            i++;
        }
    }
}

double performSerialOperationOnLinkedList(double mMember, double mInsert, double mDelete)
{
    PopulateLinkedList(1000, &head_serial);
    int m = 10000;

    clock_t start = clock();
    serialOperation(m, mMember, mInsert, mDelete);
    clock_t end = clock();

    double times = ((double)(end - start)) / CLOCKS_PER_SEC;

    FreeLinkedList(&head_serial);
    memberCountSerial = 0;
    insertCountSerial = 0;
    deleteCountSerial = 0;

    return times;
}
