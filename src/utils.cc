#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>

#include "utils.h"

bool queue_empty(CircularQueue* queue) {
    if (queue->inoffset == queue->outoffset) {
        return true;
    }
    return false;
}

bool queue_full(CircularQueue* queue) {
    if (queue_size(queue) == MAXLINE-1) {
        return true;
    }
    return false;
}

int queue_size(CircularQueue* queue) {
    return (queue->inoffset + MAXLINE - queue->outoffset) % MAXLINE;
}

int delete_frame(CircularQueue* queue, int size) {
    //not used now
    return 0;
}

int read_from_soc(CircularQueue* queue, int soc) {
    int left_bytes = std::min(MAXLINE - queue->inoffset,
            (queue->outoffset + MAXLINE - queue->inoffset - 1) % MAXLINE);
    int nbytes = read(soc, queue->buffer + queue->inoffset, left_bytes);
    if (nbytes > 0) {
        queue->inoffset = (queue->inoffset + nbytes) % MAXLINE;
    }
    return nbytes;
}

int write_to_soc(CircularQueue* queue, int soc) {
    int writen_bytes = std::min(MAXLINE - queue->outoffset,
            (queue->inoffset + MAXLINE - queue->outoffset) % MAXLINE);
    int nbytes = write(soc, queue->buffer + queue->outoffset, writen_bytes);
    if (nbytes > 0) {
        queue->outoffset = (queue->outoffset + nbytes) % MAXLINE;
    }
    return nbytes;
}
