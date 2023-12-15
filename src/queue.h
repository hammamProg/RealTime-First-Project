#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// Structure for the queue
struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};

struct Queue* createQueue(unsigned capacity); // Function to create a new queue
int isFull(struct Queue* queue); // Function to check if the queue is full
int isEmpty(struct Queue* queue); // Function to check if the queue is empty
void enqueue(struct Queue* queue, int item); // Function to enqueue an element to the queue
int dequeue(struct Queue* queue); // Function to dequeue an element from the queue
void displayQueue(struct Queue* queue); // Function to display the elements of the queue
void freeQueue(struct Queue* queue); // Function to free the memory used by the queue
