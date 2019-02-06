#include "gc.h"
#include <assert.h>
#include <stdio.h>

void printTestCase(char *s) { printf("Starting test case: %s\n", s); }

void printEndTestCase() { printf("\n====\n\n\n"); }

void testNewVM() {
  printTestCase("Initializing VM");
  newVM();
  printEndTestCase();
}

void testNewInt() {
  printTestCase("new int");
  VM *vm = newVM();
  Object *o = newInt(vm, 7);
  printObject(o);
  assert(o->value == 7);
  printEndTestCase();
}

void testNewPair() {
  printTestCase("new pair");
  VM *vm = newVM();
  Object *head = newInt(vm, 1);
  Object *tail = newInt(vm, 2);
  Object *o = newReferencePair(vm, head, tail);
  printObject(o);
  printEndTestCase();
}

void testNewPair_Reference() {
  printTestCase("new pair with reference");
  VM *vm = newVM();
  Object *head = newInt(vm, 0);
  Object *tail = newInt(vm, 2);
  Object *mid = newReferencePair(vm, head, tail);
  Object *o = newReferencePair(vm, head, mid);
  printObject(o);
  printEndTestCase();
}

void testVmStackTests() {
  printTestCase("VM stack test cases");
  VM *vm = newVM();
  Object *head = newInt(vm, 1);
  Object *tail = newInt(vm, 2);
  Object *o = newReferencePair(vm, head, tail);
  assert(4 == vm->stackSize);
  popFromVM(vm);
  assert(3 == vm->stackSize);
  for (int i = 0; i < vm->stackSize; i++) {
    printObject(vm->stack[i]);
  }
  printEndTestCase();
}

void testVmLinkedList() {
  printTestCase("VM LinkedList test cases (all objects in the LL)");
  VM *vm = newVM();
  Object *head = newInt(vm, 1);
  Object *tail = newInt(vm, 2);
  Object *o = newReferencePair(vm, head, tail);
  assert(4 == vm->stackSize);
  popFromVM(vm);
  assert(3 == vm->stackSize);

  int counter = 0;
  ObjectLinkedList *current = vm->head;
  while (current != NULL) {
    counter += 1;
    printObject(current->object);
    current = current->next;
  }
  assert(4 == counter); // even though the stack size is 2 make sure that
  // there are 3 items in the linked list
  printEndTestCase();
}

void testFrameMarkers() {
  printTestCase("Frame marker test cases");
  VM *vm = newVM();
  pushFrame(vm);
  newInt(vm, 1);
  newInt(vm, 2);
  assert(4 == vm->stackSize);
  pushFrame(vm);
  newInt(vm, 1);
  newInt(vm, 2);
  assert(7 == vm->stackSize);
  for (int i = 0; i < vm->stackSize; i++) {
    printObject(vm->stack[i]);
  }
  printf("\n");
  popFrame(vm);
  assert(4 == vm->stackSize);
  for (int i = 0; i < vm->stackSize; i++) {
    printObject(vm->stack[i]);
  }
  printf("\n");
  popFrame(vm);
  assert(1 == vm->stackSize);
  for (int i = 0; i < vm->stackSize; i++) {
    printObject(vm->stack[i]);
  }
  printf("\n");
  printEndTestCase();
}

int main() {
  testNewVM();
  testNewInt();
  testNewPair();
  testNewPair_Reference();
  testVmStackTests();
  testVmLinkedList();
  testFrameMarkers();
}
