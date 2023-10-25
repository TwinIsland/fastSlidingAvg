#include "queue.h"

// Define a tail queue
TAILQ_HEAD(tailhead, Node) head;

// Pre-allocated nodes
struct Node *nodes;
int _n_node;
int freeNodeIndex = 0;

// Initialize pre-allocated nodes
void initializeQueue(int n_node)
{   
    _n_node = n_node;

    nodes = malloc(sizeof(struct Node) * _n_node);
    if (nodes == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    TAILQ_INIT(&head);

    // Initialize node values
    for (int i = 0; i < _n_node; ++i)
    {
        nodes[i].value = 0;
    }

    // printf("queue initialized\n");
}

// Function to add an element to the end of the queue
void enqueue(double val)
{
    if (freeNodeIndex >= _n_node)
    {
        fprintf(stderr, "No more free nodes.\n");
        exit(1);
    }

    // printf("queue size: %d\n", freeNodeIndex);
    struct Node *node = &nodes[freeNodeIndex++];
    node->value = val;
    TAILQ_INSERT_TAIL(&head, node, entries);
}

// Function to remove an element from the front of the queue
double dequeue()
{
    struct Node *node = TAILQ_FIRST(&head);
    if (node == NULL)
    {
        fprintf(stderr, "No more nodes\n");
        exit(1);
    }

    TAILQ_REMOVE(&head, node, entries);
    --freeNodeIndex; // Decrement index of free node
    return node->value;
}

void freeQueue() {
    free(nodes);
}