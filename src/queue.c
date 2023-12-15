#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

// Function to create a new queue
struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) {
        perror("Failed to create queue");
        exit(EXIT_FAILURE);
    }
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;  // Important: Initialize rear as the last index
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    if (queue->array == NULL) {
        perror("Failed to create queue array");
        exit(EXIT_FAILURE);
    }
    return queue;
}

// Function to check if the queue is full
int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

// Function to check if the queue is empty
int isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

// Function to enqueue an element to the queue
void enqueue(struct Queue* queue, int item) {
    if (isFull(queue)) {
        fprintf(stderr, "Queue is full, cannot enqueue\n");
        exit(EXIT_FAILURE);
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to dequeue an element from the queue
int dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Queue is empty, cannot dequeue\n");
        exit(EXIT_FAILURE);
    }
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to display the elements of the queue
void displayQueue(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return;
    }
    int i;
    for (i = queue->front; i <= queue->rear; i++)
        printf("%d ", queue->array[i]);
    printf("\n");
}

// Function to free the memory used by the queue
void freeQueue(struct Queue* queue) {
    free(queue->array);
    free(queue);
}

// int main() {
//     struct Queue* myQueue = createQueue(MAX_SIZE);

//     int myArray[] = {1, 2, 3, 4, 5};
//     size_t arraySize = sizeof(myArray) / sizeof(myArray[0]);

//     // Enqueue each element of the array
//     for (size_t i = 0; i < arraySize; ++i) {
//         enqueue(myQueue, myArray[i]);
//     }

//     // Display the elements in the queue
//     printf("Queue elements: ");
//     displayQueue(myQueue);

//     // Dequeue an element
//     int dequeuedItem = dequeue(myQueue);
//     printf("Dequeued item: %d\n", dequeuedItem);

//     // Display the elements after dequeue
//     printf("Queue elements after dequeue: ");
//     displayQueue(myQueue);

//     // Free the memory used by the queue
//     freeQueue(myQueue);

//     return 0;
// }
