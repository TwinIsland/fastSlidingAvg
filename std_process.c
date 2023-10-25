// std_process.c
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "std_process.h"
#include <string.h>

SlidingWindow* sliding_window_create(int window_size, int hop_size) {
    SlidingWindow *sw = (SlidingWindow *)malloc(sizeof(SlidingWindow));
    if (!sw) {
        perror("Failed to allocate memory for SlidingWindow");
        exit(EXIT_FAILURE);
    }
    sw->window_size = window_size;
    sw->hop_size = hop_size;
    sw->window_t_price = 0;
    sw->n_window_item = 0;
    initializeQueue(sw->frame_t_price_queue, window_size / hop_size);
    initializeQueue(sw->n_frame_item_queue, window_size / hop_size);
    return sw;
}

void sliding_window_destroy(SlidingWindow *sw) {
    // Destroy queue
    freeQueue(sw->frame_t_price_queue);
    freeQueue(sw->n_frame_item_queue);
    free(sw);
}

void process_trade(SlidingWindow *sw, long time, double price, double volume) {
    static int init_step_flag = 1;
    static int queue_count = 0; // Debugging: count queue size
    static double frame_t_price = 0.0;
    static int n_frame_item = 0;

    price *= volume;

    if (init_step_flag) {
        sw->cur_frame_starttime = time;
        sw->deadline = time + sw->window_size;
        init_step_flag = 0;
    }

    if (time <= sw->deadline) {
        if (time < sw->cur_frame_starttime + sw->hop_size) {
            frame_t_price += price;
            n_frame_item++;
        } else {
            enqueue(sw->frame_t_price_queue, frame_t_price);
            sw->window_t_price += frame_t_price;
            sw->n_window_item += n_frame_item;

            queue_count++; // Debugging

            // append zeros for missing frames
            for (long i = sw->cur_frame_starttime + sw->hop_size; i < time; i += sw->hop_size) {
                enqueue(sw->frame_t_price_queue, 0);
                queue_count++; // Debugging
                sw->cur_frame_starttime += sw->hop_size;
            }

            // reset frame variables
            n_frame_item = 1;
            frame_t_price = price;
            sw->cur_frame_starttime += sw->hop_size;
        }
    }

    assert(queue_count == sw->window_size / sw->hop_size);
}

double compute_average(SlidingWindow *sw) {
    return sw->window_t_price / sw->n_window_item;
}

int main() {
    char *file = "/Users/yunfanhu/Documents/AlgoTrading/hudsonthames/Implementations/FML-DataStructures/FML/processed-data/30-days/ETHUSDT-trades-2023-09.csv";

    FILE *fp = fopen(file, "r");
    if (fp == NULL) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    SlidingWindow *sw = sliding_window_create(24 * 60 * 60 * 1000, 1000);

    char line[128];
    char *token;
    long time;
    double price, volume;

    fgets(line, sizeof(line), fp);  // Skip header if any
    while (fgets(line, sizeof(line), fp)) {
        token = strtok(line, ",");
        time = atol(token);

        token = strtok(NULL, ",");
        price = atof(token);

        token = strtok(NULL, ",");
        volume = atof(token);

        process_trade(sw, time, price, volume);
    }

    printf("%f\n", compute_average(sw));

    sliding_window_destroy(sw);
    fclose(fp);

    return 0;
}
