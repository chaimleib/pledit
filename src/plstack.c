#include "plstack.h"

pl_stack *pl_stack_create(plist_t node) {
  pl_stack *result = malloc(sizeof(pl_stack));
  if (!result) {
    return NULL;
  }
  result->parent = NULL;
  result->node = node;
  result->index = 0;
  result->size = 0;
  result->iter = NULL;

  plist_type nodeType = plist_get_node_type(node);
  switch (nodeType) {
  case PLIST_DICT:
    result->size = plist_dict_get_size(node);
    plist_dict_new_iter(node, result->iter);
    break;
  case PLIST_ARRAY:
    result->size = plist_array_get_size(node);
    break;
  default:
    free(result);
    return NULL;
  }
  return result;
}

pl_stack *pl_stack_push(pl_stack *stack, plist_t child) {
  pl_stack *result = pl_stack_create(child);
  if (!result) {
    return NULL;
  }
  result->parent = stack;
  return result;
}


pl_stack *pl_stack_pop(pl_stack *stack, plist_t node) {
  node = NULL;
  if (!stack) {
    return NULL;
  }
  node = stack->node;
  return stack->parent;
}

