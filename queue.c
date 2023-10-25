#include "queue.h"

// Initialize pre-allocated nodes
void initializeQueue(struct Queue *q, int n_node)
{   
    q->_n_node = n_node;

    q->nodes = malloc(sizeof(struct Node) * q->_n_node);
    if (q->nodes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    TAILQ_INIT(&q->head);

    // Initialize node values
    for (int i = 0; i < q->_n_node; ++i)
    {
        q->nodes[i].value = 0;
    }
    q->freeNodeIndex = 0;
}

// Function to add an element to the end of the queue
void enqueue(struct Queue *q, double val)
{
    if (q->freeNodeIndex >= q->_n_node)
    {
        fprintf(stderr, "No more free nodes.\n");
        exit(1);
    }

    struct Node *node = &q->nodes[q->freeNodeIndex++];
    node->value = val;
    TAILQ_INSERT_TAIL(&q->head, node, entries);
}

// Function to remove an element from the front of the queue
double dequeue(struct Queue *q)
{
    struct Node *node = TAILQ_FIRST(&q->head);
    if (node == NULL)
    {
        fprintf(stderr, "No more nodes\n");
        exit(1);
    }

    TAILQ_REMOVE(&q->head, node, entries);
    --q->freeNodeIndex;
    return node->value;
}

// Function to free allocated memory for the queue
void freeQueue(struct Queue *q) {
    free(q->nodes);
}
