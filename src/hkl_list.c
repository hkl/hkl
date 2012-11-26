#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_list.h"

static HklListNode* hkl_listnode_new(void* data)
{

  HklListNode* node = hkl_alloc_object(HklListNode);

  node->data = data;
  node->next = NULL;
  node->last = NULL;

  return node;
}

HklList* hkl_list_new()
{
  HklList* list = hkl_alloc_object(HklList);

  list->head = list->tail = NULL;
  list->size = 0;

  return list;
}

void hkl_list_free(HklList* list)
{
  assert(list != NULL);

  hkl_list_clear(list);

  hkl_free_object(list);
}

static void hkl_listnode_free(HklListNode* node)
{
  assert(node != NULL);
  hkl_free_object(node);
}


void hkl_list_push_back(HklList *list, void *data)
{
  assert(list != NULL);
  list->size++;

  HklListNode *node = hkl_listnode_new(data);

  //if there is one or more
  if(list->tail != NULL)
  {
    //point the nodes last pointer to the back node or NULL 
    node->last = list->tail;
    list->tail->next = node; 
  }

  //if there is nothing
  if (list->head == NULL)
   list->head = node;  

  list->tail = node;
}

void* hkl_list_pop_back(HklList *list)
{
  assert(list != NULL);
  void* data = NULL;

  // if there is one or more
  if(list->tail != NULL)
  {
    list->size--;
    HklListNode *node = list->tail;

    // if there is more than one
    if (list->tail->last != NULL) 
    {
      list->tail = node->last;
      list->tail->next = NULL;
    }
    else 
    {
      // if there is only one
      list->head = NULL;
      list->tail = NULL;
    }

    data = node->data;
    hkl_listnode_free(node);
  }
  return data;
}

void hkl_list_push_front(HklList *list, void *data)
{
  assert(list != NULL);

  list->size++;

  HklListNode *node = hkl_listnode_new(data);

  //if there is one or more
  if(list->head != NULL)
  {
    //point the nodes next pointer to the front node or NULL 
    node->next = list->head;
    list->head->last = node; 
  }

  //set the head of the list to the new node
  list->head = node;

  //if there is nothing
  if (list->tail == NULL)
    list->tail = node;  
}

void* hkl_list_pop_front(HklList *list) 
{
  assert(list != NULL);

  //set node to the front of the list 
  HklListNode *node = list->head;
  void* data = NULL; 

//if there is one or more
  if (list->head != NULL) 
  {
    list->size--;

    // if there is more than one 
    if(list->head->next != NULL)
    {
     list->head = list->head->next;
     list->head->last = NULL;
    }
    else 
    {
      //if there is only one 
      list->head = NULL;
      list->tail = NULL;
    }
    
    //get the data from the node you removed
    data = node->data;
    hkl_listnode_free(node);
  } 
    return data;
}


void hkl_list_clear(HklList* list)
{
  assert(list != NULL);

  HklListNode* node = list->head;
  HklListNode* temp;

  while (node != NULL)
  {
    temp = node->next;
    hkl_listnode_free(node);
    node = temp;
  }

  list->head = list->tail = NULL;
  list->size = 0;
}

void hkl_list_traverse(HklList* list, bool(*fn)(void*, void*), void* data)
{
  HklListNode* node = list->head;
  while (node != NULL)
  {
    if (fn(node->data, data))
      break;
    node = node->next;
  }
}