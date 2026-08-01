#include"ring_buffer.h"
#include <string.h>

//----------------------------------------------------------------------------------------------

void init_ring_buffer(struct ring_buffer* defined_ringbuffer)
{
    defined_ringbuffer->put = 0;
    defined_ringbuffer->get = 0;
    memset(defined_ringbuffer->array,0,RING_BUFFER_SIZE);
}

//----------------------------------------------------------------------------------------------

bool is_empty(struct ring_buffer* defined_ringbuffer)
{
    return (defined_ringbuffer->put == defined_ringbuffer->get);
}

bool is_full(struct ring_buffer* defined_ringbuffer)
{
    return ((((defined_ringbuffer->put) + 1) % RING_BUFFER_SIZE) == defined_ringbuffer->get);
}

bool push(char c,struct ring_buffer* defined_ringbuffer)
{
    bool push_check = is_full(defined_ringbuffer);
    if(push_check) 
    {
        return false;
    }
    
    else
    {
        defined_ringbuffer->array[defined_ringbuffer->put] = c;
        defined_ringbuffer->put = ((defined_ringbuffer->put + 1) % RING_BUFFER_SIZE);
        return true;

    }
}

bool pop(struct ring_buffer* defined_ringbuffer, char* s)
{
    bool pop_check = is_empty(defined_ringbuffer);
    if(pop_check) 
    {
        return false;
    }
    
    else
    {
        *s = defined_ringbuffer->array[defined_ringbuffer->get];
        defined_ringbuffer->get = ((defined_ringbuffer->get + 1) % RING_BUFFER_SIZE);
        return true;

    }
}

