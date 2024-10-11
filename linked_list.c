#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"

bool Member(int value, struct list_node_s *head_p)
{
    while (head_p != NULL && head_p->data < value)
    {
        head_p = head_p->next;
    }
    return (head_p != NULL && head_p->data == value);
}

bool Insert(int value, struct list_node_s **head_pp)
{
    struct list_node_s **curr_pp = head_pp;
    while (*curr_pp != NULL && (*curr_pp)->data < value)
    {
        curr_pp = &(*curr_pp)->next;
    }

    if (*curr_pp == NULL || (*curr_pp)->data > value)
    {
        struct list_node_s *new_node = malloc(sizeof(struct list_node_s));
        if (new_node == NULL)
        {
            return false; // Memory allocation failed
        }
        new_node->data = value;
        new_node->next = *curr_pp;
        *curr_pp = new_node;
        return true;
    }
    return false;
}

bool Delete(int value, struct list_node_s **head_pp)
{
    struct list_node_s **curr_pp = head_pp;
    while (*curr_pp != NULL && (*curr_pp)->data < value)
    {
        curr_pp = &(*curr_pp)->next;
    }
    if (*curr_pp != NULL && (*curr_pp)->data == value)
    {
        struct list_node_s *temp = *curr_pp;
        *curr_pp = (*curr_pp)->next;
        free(temp);
        return true;
    }
    return false;
}

void PopulateLinkedList(int n, struct list_node_s **head_pp)
{
    for (int i = 0; i < n;)
    {
        if (Insert(rand() % (1 << 16), head_pp))
        {
            i++;
        }
    }
}

void FreeLinkedList(struct list_node_s **head_pp)
{
    struct list_node_s *current = *head_pp;
    while (current != NULL)
    {
        struct list_node_s *next_node = current->next;
        free(current);
        current = next_node;
    }
    *head_pp = NULL;
}