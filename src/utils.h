#ifndef _UTILS_H_
#define _UTILS_H_

#define MAXLINE 4096

struct CircularQueue {
    char buffer[MAXLINE];
    int inoffset;
    int outoffset;
    CircularQueue() : inoffset(0), outoffset(0) {}
};

//void initialize_queue(CircularQueue* queue);

bool queue_empty(CircularQueue* queue);
bool queue_full(CircularQueue* queue);
int queue_size(CircularQueue* queue);

int delete_frame(CircularQueue* queue, int size);
int read_from_soc(CircularQueue* queue, int soc);
int write_to_soc(CircularQueue* queue, int soc);
#endif
