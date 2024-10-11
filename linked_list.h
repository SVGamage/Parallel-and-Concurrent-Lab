#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

struct list_node_s
{
    int data;
    struct list_node_s *next;
};

bool Member(int value, struct list_node_s *head_p);
bool Insert(int value, struct list_node_s **head_pp);
bool Delete(int value, struct list_node_s **head_pp);
void PopulateLinkedList(int n, struct list_node_s **head_pp);
void FreeLinkedList(struct list_node_s **head);

double performSerialOperationOnLinkedList(double mMember, double mInsert, double mDelete);
double performMutexOperationOnLinkedList(double mMember, double mInsert, double mDelete, int numThreads);
double performLockOperationOnLinkedList(double mMember, double mInsert, double mDelete, int numThreads);

#endif
