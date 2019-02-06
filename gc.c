#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "gc.h"

void printObject(Object *object) {
  if (object->marked == TRUE) {
    printf("*");
  }
  if (object->objectType == START_OF_STACK) {
    printf("START_OF_STACK ");
  } else if (object->objectType == FRAME_MARKER) {
    printf("FRAME_MARKER ");
  } else if (object->objectType == VALUE) {
    printf("(Value: %d) ", object->value);
  } else if (object->objectType == REFERENCE_PAIR) {
    printf("(Head: ");
    printObject(object->head);
    printf(", ");
    printf(" Tail: ");
    printObject(object->tail);
    printf(") ");
  } else if (object->objectType == ARRAY) {
    printf("TODO!");
  }
}
Object *newObject(VM *vm, ObjectType objectType) {
  Object *object = malloc(sizeof(Object));
  pushToVM(vm, object);
  object->marked = FALSE;
  object->objectType = objectType;
  return object;
}
Object *newInt(VM *vm, int value) {
  Object *object = newObject(vm, VALUE);
  object->value = value;
  return object;
}
Object *newReferencePair(VM *vm, Object *head, Object *tail) {
  Object *object = newObject(vm, REFERENCE_PAIR);
  object->head = head;
  object->tail = tail;
  return object;
}
Object *newFrameMarker(VM *vm) {
  Object *object = newObject(vm, FRAME_MARKER);
  return object;
}

ObjectLinkedList *appendObject(ObjectLinkedList *tail, Object *object) {
  ObjectLinkedList *ll = malloc(sizeof(ObjectLinkedList));
  ll->next = NULL;
  ll->object = object;
  tail->next = ll;
  return ll;
}

VM *newVM() {
  VM *vm = malloc(sizeof(VM));
  vm->stackSize = 0;

  Object *object = malloc(sizeof(Object));
  object->objectType = START_OF_STACK;

  ObjectLinkedList *ll = malloc(sizeof(ObjectLinkedList));
  ll->next = NULL;
  ll->object = object;

  vm->tail = ll;
  vm->head = ll;
  vm->stack[vm->stackSize] = object;
  vm->stackSize++;
}
void pushToVM(VM *vm, Object *object) {
  vm->stack[vm->stackSize] = object;
  vm->stackSize++;
  vm->tail = appendObject(vm->tail, object);
  assert(vm->stackSize < MAX_STACK_SIZE);
}
Object *popFromVM(VM *vm) {
  vm->stackSize--;
  Object *toReturn = vm->stack[vm->stackSize];
  assert(vm->stackSize > 0);
  return toReturn;
}
void pushFrame(VM *vm) {
  newFrameMarker(vm); // creates a new frame marker so that we can track what's
                      // on the stack better
}

void popFrame(VM *vm) {
  while (TRUE) {
    Object *obj = popFromVM(vm);
    if (obj->objectType == START_OF_STACK) {
      printf("Popped off the start of the stack!");
      assert(1 == 0);
    } else if (obj->objectType == FRAME_MARKER) {
      return;
    }
  }
}

/* Actual garbage collection stuff here */
/* Mark section of the mark and sweep algorithm */
void mark(Object *object) {
  if (object->marked == 1)
    return; //handle cycles, just exit
  object->marked = 1;
  if (object->objectType == REFERENCE_PAIR) {
    mark(object->head);
    mark(object->tail);
  }
}
void markAll(VM *vm) {
  for (int i = 0; i < vm->stackSize; i++) {
    mark(vm->stack[i]);
  }
}

/* sweep section of the mark and sweep algorith. Removes anything that has not been marked
and unmarks anything marked */
void sweep(VM *vm) {
  ObjectLinkedList *prev = vm->head;
  ObjectLinkedList *current = prev->next;
  while (current != NULL) {
    if (current->object->marked == 0) {
      prev->next = current->next;
      free(current);
      current = prev->next;
    }
    else {
      current->object->marked = 0; //unmark the object for the next mark&sweep pass
      prev = current;
      current = current->next;
    }
  }
}

void gc(VM* vm) {
  markAll(vm);
  sweep(vm);
}

// int main() {
//   return 0;
// }

/*
Sources:
http://journal.stuffwithstuff.com/2013/12/08/babys-first-garbage-collector/
http://maplant.com/gc.html
*/
