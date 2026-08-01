#ifndef RING_BUFFER_H          
#define RING_BUFFER_H   

#include <stdint.h>
#include <stdbool.h>


#define RING_BUFFER_SIZE 50U


struct ring_buffer
{
 int get,put;
 char array[RING_BUFFER_SIZE];
};

void init_ring_buffer(struct ring_buffer* defined_ringbuffer);

bool is_empty(struct ring_buffer* defined_ringbuffer);

bool is_full(struct ring_buffer* defined_ringbuffer);

bool push(char c,struct ring_buffer* defined_ringbuffer);

bool pop(struct ring_buffer* defined_ringbuffer,char* sentence);

#endif