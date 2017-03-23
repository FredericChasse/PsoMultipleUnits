//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Utilities
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : LinkedList.c
// Author  : Frederic Chasse
// Date    : 2017-03-19
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Purpose : This file implements the Linked List object.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Notes   : None.
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "LinkedList.h"

// Public functions
//==============================================================================

INT8 LinkedList_Init (LinkedList_t *list, Node_t *head)
{
  Node_t *temp = head;
  
  if ( (list == NULL) || (head == NULL) )
  {
    return -1;
  }
  
  list->head = head;
  list->count = 1;
  list->tail = head;
  head->list = list;
  
  temp = head->next;
  
  while (temp != NULL)
  {
    list->tail = temp;
    list->count++;
    temp->list = list;
    temp = temp->next;
  }
  
  list->tail->next = NULL;
  
  return 0;
}


INT8 LinkedList_AddToEnd (LinkedList_t *list, Node_t *node)
{
  Node_t *temp;
  
  if ( (list == NULL) || (node == NULL) )
  {
    return -1;
  }
  
  if (list->count == list->max)
  {
    return -1;
  }
  
  if (list->count == 0)
  {
    list->head = node;
    list->tail = node;
  }
  else
  {
    temp = list->tail;
    temp->next = node;
    list->tail = node;
  }
  node->next = NULL;
  list->count++;
  node->list = list;
  
  return 0;
}


INT8 LinkedList_RemoveNode (LinkedList_t *list, Node_t *node)
{
  if ( (list == NULL) || (node == NULL) )
  {
    return -1;
  }
  
  Node_t *temp;
  
  if (list->head == list->tail) // Only one element
  {
    if (node == list->head)
    {
      list->head = list->tail = NULL;
      list->count = 0;
      node->list = NULL;
      return 0;
    }
    else  // Node is not in list!
    {
      return -1;
    }
  }
  else  // List has multiple elements
  {
    temp = list->head;
    
    while ( (temp->next != node) && (temp != NULL) )
    {
      temp = temp->next;
    }
    if (temp == NULL)  // Node not in list!
    {
      return -1;
    }
    
    temp->next = node->next;
    list->count--;
    
    if (list->tail == node)
    {
      list->tail = temp;
    }
    
    node->list = NULL;
    
    return 0;
  }
}