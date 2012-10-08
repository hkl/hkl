#ifndef HKL_DEQUE_H
#define HKL_DEQUE_H

#include <stddef.h>

typedef struct HklDequeNode {

  struct HklDequeNode* next;
  void* data;

} HklDequeNode;

/**
@struct HklDeque A deque that just works.

@authors Scott LaVigne
@date 10/7/2012
*/
typedef struct HklDeque {

  HklDequeNode* head;
  HklDequeNode* tail;
  size_t size;

} HklDeque;

HklDeque* hkl_deque_new();

void hkl_deque_push_back(HklDeque* deque, void* data);

void* hkl_deque_pop_front(HklDeque* deque);

void hkl_deque_free(HklDeque* deque);

void hkl_deque_clear(HklDeque* deque);

#endif // HKL_DEQUE_H
