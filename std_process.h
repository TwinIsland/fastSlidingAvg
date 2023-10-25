// std_process.h
#ifndef STD_PROCESS_H
#define STD_PROCESS_H

#include "queue.h"

typedef struct SlidingWindow {
    int window_size;
    int hop_size;
    double window_t_price;
    int n_window_item;
    long cur_frame_starttime;
    long deadline;
    struct Queue * frame_t_price_queue;
    struct Queue * n_frame_item_queue;
} SlidingWindow;

SlidingWindow* sliding_window_create(int window_size, int hop_size);
void sliding_window_destroy(SlidingWindow *sw);
void process_trade(SlidingWindow *sw, long time, double price, double volume);
double compute_average(SlidingWindow *sw);

#endif  // STD_PROCESS_H
