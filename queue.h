#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

// Define a structure for queue entries
struct Node
{
    double value;
    TAILQ_ENTRY(Node) entries; // Tail queue
};

// Function prototypes
void initializeQueue(int n_node);
void enqueue(double val);
double dequeue();
void freeQueue();

#endif /* QUEUE_H */
