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
Push an object onto the front of the queue

@param deque The pointer of the queue to push into.
@param data A pointer to the data
@brief This is the same as a push onto a stack.
*/
void hkl_deque_push_front(HklDeque* deque, void* data);

/**
Pop an item from the front of the queue.

@param deque The deque to pop from.
@retval void* 
@breif Same as dequeue.
*/
void* hkl_deque_pop_front(HklDeque* deque);

/**
Pop an item from the back of the queue.

@param deque The deque to pop from.
@retval void* A pointer to the data being returned(not typed)
@brief Return the back of the deque's pointer and remove it from the queue
*/
void* hkl_deque_pop_back(HklDeque* deque);

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

/**
Find the nth value of a deque.

@param deque The deque that contains the item
@param n The index of the location being searched for.
@retval void* The data at the nth location

@brief This function gives the ability to find
       any item in the queue by index. It is useful
       for locating the data AND for changing that data (as it is returned by ptr).
       Lastly, all indexes are garunteed valid because it will wrap around the array meaning
       it is possible to create cyclical array by just indexing at higher values.
*/
void *hkl_deque_findn(HklDeque* deque, int n);
#endif // HKL_DEQUE_H
