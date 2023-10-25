#include "queue.h"

int main() {
    initializeQueue(12);

    enqueue(12);
    printf("number is %f\n", dequeue());
    printf("number is %f\n", dequeue());
    freeQueue();
}