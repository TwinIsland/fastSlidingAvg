#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include <assert.h>

static int WINDOW_SIZE = 24 * 60 * 60 * 1000;
static int HOP_SIZE = 60 * 1000;

static char *file = "data/ETHUSDT-trades-2021-10.csv";

long cur_frame_starttime, prev_time;

double window_t_price = 0;
double n_window_item = 0;

struct Queue frame_t_price_queue, n_frame_item_queue;

int main()
{
    FILE *fp = fopen(file, "r");
    if (fp == NULL)
    {
        printf("Could not open file\n");
        return 1;
    }

    // initialize queue
    initializeQueue(&frame_t_price_queue, WINDOW_SIZE / HOP_SIZE);
    initializeQueue(&n_frame_item_queue, WINDOW_SIZE / HOP_SIZE);

    char line[128];
    char *token;

    long time;
    double price, volume;

    double frame_t_price = 0.0;
    double n_frame_item = 0;

    int init_step_flag = 1;
    long deadline;

    int queue_count = 0; // Debugging: count queue size

    fgets(line, sizeof(line), fp); // Skip header if any

    // initialize the queue
    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, ",");
        time = atol(token);

        token = strtok(NULL, ",");
        price = atof(token);

        token = strtok(NULL, ",");
        volume = atof(token);

        // Algo to compute price
        price *= volume;

        if (init_step_flag)
        {
            // init tracking variables
            cur_frame_starttime = time;
            deadline = time + WINDOW_SIZE;
            init_step_flag = 0;
        }

        if (time <= deadline)
        {
            if (time < cur_frame_starttime + HOP_SIZE)
            {
                frame_t_price += price;
                n_frame_item++;
            }
            else
            {
                enqueue(&frame_t_price_queue, frame_t_price);
                enqueue(&n_frame_item_queue, n_frame_item);
                window_t_price += frame_t_price;
                n_window_item += n_frame_item;

                queue_count++; // Debugging

                // append zeros for missing frames
                for (long i = prev_time + HOP_SIZE; i < time; i += HOP_SIZE)
                {
                    enqueue(&frame_t_price_queue, 0);
                    enqueue(&n_frame_item_queue, 0);
                    queue_count++; // Debugging
                    cur_frame_starttime += HOP_SIZE;
                }

                // reset frame variables
                n_frame_item = 1;
                frame_t_price = price;
                cur_frame_starttime += HOP_SIZE;
            }

            prev_time = time;
        }
        else
            break;
    }

    // append zeros for missing frames
    for (long i = cur_frame_starttime + HOP_SIZE; i <= deadline; i += HOP_SIZE)
    {
        enqueue(&frame_t_price_queue, 0);
        enqueue(&n_frame_item_queue, 0);
        queue_count++; // Debugging
    }

    // reset frame variables
    cur_frame_starttime = time;
    n_frame_item = 1;
    frame_t_price = price;

    // check initializing step
    assert(queue_count == WINDOW_SIZE / HOP_SIZE);
    printf("%f\n", window_t_price / n_window_item);

    // sliding the window
    while (fgets(line, sizeof(line), fp))
    {
        token = strtok(line, ",");
        time = atol(token);

        token = strtok(NULL, ",");
        price = atof(token);

        token = strtok(NULL, ",");
        volume = atof(token);

        // Algo to compute price
        price *= volume;

        if (time < cur_frame_starttime + HOP_SIZE)
        {
            frame_t_price += price;
            n_frame_item++;
        }
        else
        {
            // new window average
            window_t_price += frame_t_price - dequeue(&frame_t_price_queue);
            n_window_item += n_frame_item - dequeue(&n_frame_item_queue);

            // printf("asdad: %f %f %f\n", window_t_price, n_window_item, n_frame_item);

            enqueue(&frame_t_price_queue, frame_t_price);
            enqueue(&n_frame_item_queue, n_frame_item);

            printf("%f\n", window_t_price / n_window_item);

            // append zeros for missing frames
            for (long i = prev_time + HOP_SIZE; i < time; i += HOP_SIZE)
            {
                // new window average
                window_t_price -= dequeue(&frame_t_price_queue);
                n_window_item -= dequeue(&n_frame_item_queue);

                enqueue(&frame_t_price_queue, 0);
                enqueue(&n_frame_item_queue, 0);

                printf("%f\n", window_t_price / n_window_item);

                cur_frame_starttime += HOP_SIZE;
            }

            // reset frame variables
            n_frame_item = 1;
            frame_t_price = price;
            cur_frame_starttime += HOP_SIZE;
        }
        prev_time = time;
    }

    freeQueue(&frame_t_price_queue);
    freeQueue(&n_frame_item_queue);
    fclose(fp); // Close the file

    return 0;
}
