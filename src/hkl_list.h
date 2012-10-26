#ifndef HKL_LIST_H
#define HKL_LIST_H

#include <stddef.h>

typedef struct HklListNode {

  struct HklListNode* next;
  struct HklListNode* last;
  void* data;

} HklListNode;

/**
@struct HklList Linked List

Data can be inserted at the beginning and end of the list. 
Data can be retrieved and removed at the begining and end of the array.
The list has pointers to the beginning and end. 

@authors Jennifer Coryell
@date 10/22/2012
*/
typedef struct HklList {

  HklListNode* head;
  HklListNode* tail;
  size_t size;

} HklList;

/**
Allocate a new HklList.

@retval HklList* A new list object.
@brief Allocates a new empty list.
*/
HklList* hkl_list_new();

/**
Push an object to the end of the list.

@param list The list to push into.
@param data A pointer to store.
*/
void hkl_list_push_back(HklList *list, void *data);

/**
Remove an object from the end of the list.

@param list The list to remove the last object from.
@retval void*
@param data A pointer to store.
*/
void* hkl_list_pop_back(HklList *list);

/**
Push an object to the front of the list.

@param list The list to push into.
@param data A pointer to store.
*/
void hkl_list_push_front(HklList *list, void *data);

/**
Remove an object from the front of the list.

@param list The list to remove from.
@retval void* 
*/
void* hkl_list_pop_front(HklList *list);

/**
Free a list.

@param list The list to empty.
*/
void hkl_list_free(HklList* list);

/**
Empty all data from a list.

@param list The list to empty.
*/
void hkl_list_clear(HklList* list);

#endif // HKL_LIST_H