/*
 * plstack.h
 *
 * Structs and functions for depth-first traversal of plists, using a
 * dynamically-allocated stack to track our progress instead of recursion. This
 * makes progress indication easier, and allows access to top-level resources
 * without global variables or argument passing through a recursive call chain.
 * In debugging, stack traces become much smaller as well.
 */
#ifndef _PLSTACK_H
#define _PLSTACK_H
#include <stdlib.h>
#include <plist/plist.h>

typedef struct {
  void *parent;
  plist_t node;
  uint32_t index;
  uint32_t size;
  plist_dict_iter *iter;
} pl_stack;

pl_stack *pl_stack_create(plist_t node);
pl_stack *pl_stack_push(pl_stack *stack, plist_t child);
pl_stack *pl_stack_pop(pl_stack *stack, plist_t node);

#endif
