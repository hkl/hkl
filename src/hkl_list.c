#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_list.h"

static HklListNode* hkl_listnode_new(void* data)
{

  HklListNode* node = hkl_alloc_object(HklListNode);

  node->data = data;
  node->next = NULL;

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

  if(list->head == NULL)
    list->head = node;
  else 
    list->tail->next = node;

  list->tail = node; 
}

void* hkl_list_pop_back(HklList *list)
{
  assert(list != NULL);
  void* data = NULL;

  if(list->tail != NULL)
  {
    list->size--;
    HklListNode *temp = list->head;

    // there is no back pointers, need to iterate 
    // through the list to set the tail pointer 
    while (temp->next != list->tail)    
      temp = temp->next;

    data = temp->next->data;
    hkl_listnode_free(temp->next);
    list->tail = temp;
  }
  return data;
}

void hkl_list_push_front(HklList *list, void *data)
{
  assert(list != NULL);

  list->size++;

  HklListNode *node = hkl_listnode_new(data);

  node->next = list->head;
  list->head = node;

  if (list->tail == NULL)
    list->tail = node;  
}

void* hkl_list_pop_front(HklList *list) 
{
  assert(list != NULL);

  HklListNode *node = list->head;
  void* data = NULL; 

  if (node != NULL) 
  {
    list->size--;
    list->head = node->next;
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
    list->size = 0; 
  }
}