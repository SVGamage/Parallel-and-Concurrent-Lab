#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;

void insert_serial(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;

    if (head == NULL || head->data > value) {
        new_node->next = head;
        head = new_node;
        return;
    }

    Node* current = head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;
}

int member_serial(int value) {
    Node* current = head;
    while (current != NULL && current->data <= value) {
        if (current->data == value) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void delete_serial(int value) {
    if (head == NULL) return;

    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        free(temp);
        return;
    }

    Node* current = head;
    while (current->next != NULL && current->next->data != value) {
        current = current->next;
    }

    if (current->next != NULL) {
        Node* temp = current->next;
        current->next = temp->next;
        free(temp);
    }
}

void populate_list(int n) {
    for (int i = 0; i < n; i++) {
        int value;
        do {
            value = rand() % 65536;  // 2^16
        } while (member_serial(value));
        insert_serial(value);
    }
}

void perform_operations_serial(int m, double m_member, double m_insert, double m_delete) {
    for (int i = 0; i < m; i++) {
        double op = (double)rand() / RAND_MAX;
        int value = rand() % 65536;

        if (op < m_member) {
            member_serial(value);
        } else if (op < m_member + m_insert) {
            if (!member_serial(value)) {
                insert_serial(value);
            }
        } else {
            delete_serial(value);
        }
    }
}

// int main() {
//     srand(time(NULL));

//     int n = 1000;
//     int m = 10000;
//     double m_member = 0.99;
//     double m_insert = 0.005;
//     double m_delete = 0.005;

//     // Populate the list
//     populate_list(n);

//     // Perform operations and measure time
//     struct timespec start, end;
//     clock_gettime(CLOCK_MONOTONIC, &start);
    
//     perform_operations(m, m_member, m_insert, m_delete);

//     clock_gettime(CLOCK_MONOTONIC, &end);

//     double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//     printf("Time taken: %f seconds\n", time_taken);

//     // Free the list
//     while (head != NULL) {
//         Node* temp = head;
//         head = head->next;
//         free(temp);
//     }

//     return 0;
// }