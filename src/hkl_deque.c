#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_deque.h"

static HklDequeNode* hkl_dequenode_new(void* data)
{

  HklDequeNode* node = hkl_alloc_object(HklDequeNode);

  node->data = data;
  node->next = NULL;

  return node;
}

static void hkl_dequenode_free(HklDequeNode* node)
{
  assert(node != NULL);
  hkl_free_object(node);
}

HklDeque* hkl_deque_new()
{
  HklDeque* deque = hkl_alloc_object(HklDeque);

  deque->head = deque->tail = NULL;
  deque->size = 0;

  return deque;
}

void hkl_deque_push_back(HklDeque* deque, void* data)
{
  assert(deque != NULL);
  deque->size++;

  HklDequeNode* node = hkl_dequenode_new(data);

  if (deque->head == NULL)
    deque->head = node;
  else
    deque->tail->next = node;

  deque->tail = node;
}

void* hkl_deque_pop_front(HklDeque* deque)
{
  assert(deque != NULL);

  HklDequeNode* node = deque->head;
  void* data = NULL;

  if (node != NULL)
  {
    deque->size--;
    data = node->data;
    deque->head = node->next;
    hkl_dequenode_free(node);
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

  HklDequeNode* node = deque->head;
  HklDequeNode* temp;

  while (node != NULL)
  {
    temp = node->next;
    hkl_dequenode_free(node);
    node = temp;
  }

  deque->size = 0;
}
