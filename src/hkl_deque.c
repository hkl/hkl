#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_deque.h"

void hkl_deque_grow(HklDeque* deque)
{
  assert(deque);

  if((deque->back == deque->max-1) && (deque->front == 0))//special case
  {
    deque->queue = realloc(deque->queue, deque->max * 2 * sizeof(void*));
  }
  else if(deque->back < deque->front)//back chunk is first part of array, front is last
  {
    size_t sizeOfFrontChunk = deque->max - deque->front;
    size_t sizeOfBackChunk = deque->max - sizeOfFrontChunk;

    void **tmp = malloc(deque->max * 2 * sizeof(void*));

    memcpy(tmp, deque->queue + deque->front, sizeOfFrontChunk * sizeof(void*));
    memcpy(tmp + sizeOfFrontChunk, deque->queue, sizeOfBackChunk * sizeof(void*));
    free(deque->queue);
    deque->queue = tmp;
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
  else
  {
    printf("dont grow broseph\n");
  }

  if(deque->size > 0)
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

void* hkl_deque_pop_front(HklDeque* deque)
{
  assert(deque != NULL);
  void* data = NULL;
  
  if (deque->size > 0)
  {
    data = deque->queue[deque->front];
    //deque->queue[deque->front] = NULL;

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

void hkl_deque_free(HklDeque* deque)
{
  assert(deque != NULL);

  hkl_deque_clear(deque);

  hkl_free_object(deque);
}

void hkl_deque_clear(HklDeque* deque)
{

  assert(deque);

  free (deque->queue);
  deque->size = 0;
  deque->max = 2;
  deque->front = deque->back = 0;
  deque->queue = malloc(2 * sizeof(void*));
}

