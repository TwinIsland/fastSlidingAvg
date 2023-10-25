#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

struct Node
{
    double value;
    TAILQ_ENTRY(Node) entries; // Tail queue
};

struct Queue
{
    TAILQ_HEAD(, Node) head;
    struct Node *nodes;
    int _n_node;
    int freeNodeIndex;
};

// Function prototypes
void initializeQueue(struct Queue *q, int n_node);
void enqueue(struct Queue *q, double val);
double dequeue(struct Queue *q);
void freeQueue(struct Queue *q);

#endif /* QUEUE_H */
