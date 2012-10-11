#ifndef HKL_DEQUE_H
#define HKL_DEQUE_H

#include <stddef.h>

/**
@struct HklDeque A deque that just works.

This structure is a general purpose list that can be used
as a double-ended queue.

Data inserted into the queue is managed to a degree.
pointers stored are not freed and the objects that are
being pointed too need to be deallocated.

@authors Scott LaVigne
@date 10/7/2012
*/
typedef struct HklDeque {

  void **queue;
  size_t size, max;
  int front, back;

} HklDeque;

/**
Allocate a new HklDeque.

@retval HklDeque* A new new deque object.
@brief Allocates a new empty deque.
*/
HklDeque* hkl_deque_new();

/**
Allocate a new HklDeque with room for size enteries

@retval HklDeque* A new deque with room for size enteries
@param size_t The size that the queue will be initialized to hold
@brief Allocates a new empty queue. This queue is given a specific size
       and thus allows the user to optimize the queue for their purposes.
*/
HklDeque *hkl_deque_new_sized(size_t size);

/**
Push an object to the end of the queue.

@param deque The deque to push into.
@param data A pointer to store.
@brief Same as enqueue.
*/
void hkl_deque_push_back(HklDeque* deque, void* data);

/**
Pop an item from the front of the queue.

@param deque The deque to pop from.
@retval void* 
@breif Same as dequeue.
*/
void* hkl_deque_pop_front(HklDeque* deque);

/**
Free a deque.

@param deque The deque to free.
*/
void hkl_deque_free(HklDeque* deque);

/**
Empty all data from a deque.

@param deque The deque to empty.
*/
void hkl_deque_clear(HklDeque* deque);

#endif // HKL_DEQUE_H
