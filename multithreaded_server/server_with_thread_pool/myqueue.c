#include "myqueue.h"
#include <stdlib.h>
#include <stdio.h>

node_t* head = NULL;
node_t* tail = NULL;

void enqueue(int *client_socket){
    node_t *newnode = malloc(sizeof(node_t));
    newnode->client_socket = client_socket;
    newnode->next = NULL;
    if (tail == NULL) {
        head = newnode;
    }
    else {
        tail->next = newnode;
    }
    tail = newnode;
}

// return NULL if the queue is empty
// return the pointer to a client_socket, if there is one to get
int *dequeue() {
    if (head == NULL){
        return NULL;
    }
    else{
        int* client_socket = malloc(sizeof(int));
        *client_socket = *(head->client_socket);
        node_t* tmp = head;
        head = head->next;
        if (head == NULL){
            tail = NULL;
        }
        free(tmp);
        return client_socket;
    }
}