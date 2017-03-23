//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// Utilities
//
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//
// File    : LinkedList.h
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

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "Setup.h"

// Public definitions
//==============================================================================

typedef struct
{
  struct sLinkedList_t *list;
  void *ctx;
  void *next;
  UINT8 key;    // For debug purposes
} Node_t;

typedef struct sLinkedList_t
{
  Node_t *head;
  Node_t *tail;
  size_t  count;
  size_t  max;
} LinkedList_t;

// Public functions
//==============================================================================

INT8 LinkedList_Init      (LinkedList_t *list, Node_t *head);
INT8 LinkedList_AddToEnd  (LinkedList_t *list, Node_t *node);
INT8 LinkedList_RemoveNode(LinkedList_t *list, Node_t *node);



#endif // __LINKED_LIST_H__