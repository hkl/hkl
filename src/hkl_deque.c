#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_deque.h"

void hkl_deque_grow(HklDeque* deque)
{
  assert(deque);

  if((deque->front == deque->max-1) && (deque->back == 0))
  {
    realloc(deque->queue, deque->max * sizeof(void*));
  }
  else
  {
    int backCopyCount = deque->max - deque->back - 1;
    void ** tmp = malloc(deque->max * sizeof(void*));

    memcpy(tmp, (deque->queue + deque->back), backCopyCount * sizeof(void*));
    memcpy(tmp + backCopyCount, deque->queue, (deque->front + 1) * sizeof(void*));

    free(deque->queue);
    deque->queue = tmp;
    deque->back = 0;
    deque->front = deque->max;
  }
  deque->max *= 2;
}

HklDeque* hkl_deque_new()
{
  HklDeque* deque = hkl_alloc_object(HklDeque);

  deque->size = 0;
  deque->max = 2;
  deque->front = deque->back = 0;
  deque->queue = malloc(2 * sizeof(void*));

  return deque;                             
}

void hkl_deque_push_back(HklDeque* deque, void* data)
{
  assert(deque != NULL);
  //do check here for size - if too big we need to grow
 
  deque->size++;
  deque->back = (deque->back - 1) % deque->max;
  deque->queue[deque->back] = data;
}

void* hkl_deque_pop_front(HklDeque* deque)
{
  assert(deque != NULL);

  void* data = NULL;

  if (deque->size != 0)
  {
    deque->size--;
    data = deque->queue[deque->front];
    deque->queue[deque->front] = NULL;

    if(deque->size != 0)
      deque->front = (deque->front - 1) % 5;
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

