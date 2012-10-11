#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_deque.h"

void hkl_deque_grow(HklDeque* deque)
{
  assert(deque);

  if((deque->back == deque->max-1) && (deque->front == 0))//special case of array in order
  {
    deque->queue = realloc(deque->queue, deque->max * 2 * sizeof(void*));
  }                                   
  //We need to move the chunk representing first part of the queue to the front
  //and move back of the array to the back
  else if(deque->back < deque->front)
  {
    size_t sizeOfFrontChunk = deque->max - deque->front;
    size_t sizeOfBackChunk = deque->max - sizeOfFrontChunk;

    void **tmp = malloc(deque->max * 2 * sizeof(void*));

    //only copy the front part of the queue to the front
    memcpy(tmp, deque->queue + deque->front, sizeOfFrontChunk * sizeof(void*));
    //copy the rest to the end of the queue
    memcpy(tmp + sizeOfFrontChunk, deque->queue, sizeOfBackChunk * sizeof(void*));
    free(deque->queue);
    deque->queue = tmp;

    //set our ends to their proper values
    deque->front = 0;
    deque->back = deque->max - 1;
  }
  else
  {
    assert(0);//should never reach here - queue was in an impossible state
  }

  deque->max *= 2;
}

HklDeque* hkl_deque_new()
{
  HklDeque* deque = hkl_alloc_object(HklDeque);

  deque->size = 0;
  deque->max = 1;
  deque->front = deque->back = 0;
  deque->queue = malloc(sizeof(void*));

  return deque;                             
}

HklDeque *hkl_deque_new_sized(size_t size)
{
  HklDeque* deque = hkl_alloc_object(HklDeque);

  deque->size = 0;
  deque->max = size;
  deque->front = deque->back = 0;
  deque->queue = malloc(size * sizeof(void*));

  return deque;                             
}

void hkl_deque_push_back(HklDeque* deque, void* data)
{
  assert(deque != NULL);

  if(deque->size == deque->max)
  {
    hkl_deque_grow(deque);
  }

  if(deque->size > 0)//normal case insert
  {
    deque->back = (deque->back + 1) % deque->max;
    deque->queue[deque->back] = data;
  }
  else//inserting into empty queue
  {
    deque->queue[0] = data;
  }

  ++deque->size;
}


void hkl_deque_push_front(HklDeque* deque, void* data)
{
  assert(deque != NULL);

  if(deque->size == deque->max)
  {
    hkl_deque_grow(deque);
  }

  if(deque->size > 0)//normal case insert
  {
    deque->front = (deque->front - 1) % deque->max;
    deque->queue[deque->front] = data;
  }
  else//special case of empty tree
  {
    deque->queue[0] = data;
  }

  ++deque->size;
}

void* hkl_deque_pop_front(HklDeque* deque)
{
  assert(deque != NULL);
  void* data = NULL;
  
  if (deque->size > 0)
  {
    data = deque->queue[deque->front];
    --deque->size;

    if(deque->size > 0)
    {
      deque->front = (deque->front + 1) % deque->max;
    }
    else//we just emptied the queue reset front and back
    {
      deque->front = deque->back = 0;
    }
  }

  return data;
}

void* hkl_deque_pop_back(HklDeque* deque)
{
  assert(deque != NULL);
  void *data = NULL;

  if(deque->size > 0)
  {
    data = deque->queue[deque->back];
    --deque->size;

    if(deque->size > 0)
    {
      deque->back = (deque->back - 1) % deque->max;
    }
    else//we just emptied the queue reset front and back
    {
      deque->front = deque->back = 0;
    }
  }
  
  return data;
}

void hkl_deque_free(HklDeque* deque)
{
  assert(deque != NULL);

  hkl_deque_clear(deque);

  hkl_free_object(deque);
}

void hkl_deque_clear(HklDeque* deque)
{

  assert(deque);

  free (deque->queue);//free our underlying queue
}

void *hkl_deque_findn(HklDeque *deque, int n)
{
  assert(deque != NULL);
  
  int *data = NULL;
  if(n >= 0 && n < deque->size)
  {
    int location = (deque->front + n) % deque->max;
    data = deque->queue[location];
  }
  else if(n < 0 && -n < deque->size + 1)
  {
    //we want to offset the value so that -1 is the back
    int location = (deque->back + (n + 1)) % deque->max;  
    data = deque->queue[location];
  }

  return data;
}

void *hkl_deque_findn_cyclical(HklDeque *deque, int n)
{
  assert(deque != NULL);
  int *data = NULL;

  if(deque->size > 0)//only null on an empty queue
  {
    int index = (n % deque->size) + deque->front;//get a cyclical index 
    data = deque->queue[index];
  }
  
  return data;
}
