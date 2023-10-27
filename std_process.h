// std_process.h
#ifndef STD_PROCESS_H
#define STD_PROCESS_H

#include "queue.h"
#include <stdbool.h>

typedef struct SlidingWindow {
    int window_size;
    int hop_size;
    double window_t_price;
    int n_window_item;
    long cur_frame_starttime;   
    long deadline;
    long prev_time;
    struct Queue * frame_t_price_queue;
    struct Queue * n_frame_item_queue;
} SlidingWindow;

SlidingWindow* sliding_window_create(int window_size, int hop_size);
void sliding_window_destroy(SlidingWindow *sw);
bool InitSlidingWindow(SlidingWindow *sw);
double compute_average(SlidingWindow *sw);
void pushZero(SlidingWindow *sw, bool count_queue);
void SlideTheWindow(SlidingWindow *sw);
void ComplementZero(SlidingWindow *sw);
#endif  // STD_PROCESS_H
