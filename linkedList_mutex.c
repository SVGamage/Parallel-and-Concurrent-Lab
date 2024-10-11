#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* head = NULL;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

void insert_mutex(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = value;
    new_node->next = NULL;

    pthread_mutex_lock(&list_mutex);

    if (head == NULL || head->data > value) {
        new_node->next = head;
        head = new_node;
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    Node* current = head;
    while (current->next != NULL && current->next->data < value) {
        current = current->next;
    }

    new_node->next = current->next;
    current->next = new_node;

    pthread_mutex_unlock(&list_mutex);
}

int member_mutex(int value) {
    pthread_mutex_lock(&list_mutex);

    Node* current = head;
    while (current != NULL && current->data <= value) {
        if (current->data == value) {
            pthread_mutex_unlock(&list_mutex);
            return 1;
        }
        current = current->next;
    }

    pthread_mutex_unlock(&list_mutex);
    return 0;
}

void delete_mutex(int value) {
    pthread_mutex_lock(&list_mutex);

    if (head == NULL) {
        pthread_mutex_unlock(&list_mutex);
        return;
    }

    if (head->data == value) {
        Node* temp = head;
        head = head->next;
        free(temp);
        pthread_mutex_unlock(&list_mutex);
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

    pthread_mutex_unlock(&list_mutex);
}

void populate_list_mutex(int n) {
    for (int i = 0; i < n; i++) {
        int value;
        do {
            value = rand() % 65536;  // 2^16
        } while (member_mutex(value));
        insert_mutex(value);
    }
}

typedef struct {
    int m;
    double m_member;
    double m_insert;
    double m_delete;
} ThreadArgs;

void* perform_operations_mutex(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    for (int i = 0; i < args->m; i++) {
        double op = (double)rand() / RAND_MAX;
        int value = rand() % 65536;

        if (op < args->m_member) {
            member_mutex(value);
        } else if (op < args->m_member + args->m_insert) {
            if (!member_mutex(value)) {
                insert_mutex(value);
            }
        } else {
            delete_mutex(value);
        }
    }
    return NULL;
}

// int main(int argc, char* argv[]) {
//     if (argc != 2) {
//         printf("Usage: %s <num_threads>\n", argv[0]);
//         return 1;
//     }

//     int num_threads = atoi(argv[1]);
//     srand(time(NULL));

//     int n = 1000;
//     int m = 10000;
//     double m_member = 0.99;
//     double m_insert = 0.005;
//     double m_delete = 0.005;

//     // Populate the list
//     populate_list(n);

//     pthread_t threads[num_threads];
//     ThreadArgs args;
//     args.m = m / num_threads;
//     args.m_member = m_member;
//     args.m_insert = m_insert;
//     args.m_delete = m_delete;

//     // Perform operations and measure time
//     struct timespec start, end;
//     clock_gettime(CLOCK_MONOTONIC, &start);

//     for (int i = 0; i < num_threads; i++) {
//         pthread_create(&threads[i], NULL, perform_operations_mutex, &args);
//     }

//     for (int i = 0; i < num_threads; i++) {
//         pthread_join(threads[i], NULL);
//     }

//     clock_gettime(CLOCK_MONOTONIC, &end);

//     double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
//     printf("Time taken: %f seconds\n", time_taken);

//     // Free the list
//     while (head != NULL) {
//         Node* temp = head;
//         head = head->next;
//         free(temp);
//     }

//     pthread_mutex_destroy(&list_mutex);

//     return 0;
// }