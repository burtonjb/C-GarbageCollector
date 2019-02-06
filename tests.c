#include "gc.h"
#include <assert.h>
#include <stdio.h>

void printTestCase(char *s) { printf("Starting test case: %s\n", s); }

void endTestCase(VM *vm) {
  deleteVM(vm);
  printf("\n====\n\n\n");
}

void testNewVM() {
  printTestCase("Initializing VM");
  VM *vm = newVM();
  endTestCase(vm);
}

void testNewInt() {
  printTestCase("new int");
  VM *vm = newVM();
  Object *o = newInt(vm, 7);
  printObject(o);
  assert(o->value == 7);
  endTestCase(vm);
}

void testNewPair() {
  printTestCase("new pair");
  VM *vm = newVM();
  Object *head = newInt(vm, 1);
  Object *tail = newInt(vm, 2);
  Object *o = newReferencePair(vm, head, tail);
  printObject(o);
  endTestCase(vm);
}

void testNewPair_Reference() {
  printTestCase("new pair with reference");
  VM *vm = newVM();
  Object *head = newInt(vm, 0);
  Object *tail = newInt(vm, 2);
  Object *mid = newReferencePair(vm, head, tail);
  Object *o = newReferencePair(vm, head, mid);
  printObject(o);
  endTestCase(vm);
}

void testNewArray() {
  printTestCase("New array test");
  VM *vm = newVM();
  Object *head = newInt(vm, 0);
  Object *array = newArray(vm, 5, head);
  printObject(array);
  printf("\n");
  Object *new = newInt(vm, 6);
  array->objects[3] = new;
  printObject(array);
  endTestCase(vm);
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
  endTestCase(vm);
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
  endTestCase(vm);
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
  endTestCase(vm);
}

void testMark() {
  printTestCase("Test mark algorithm");
  VM *vm = newVM();
  Object *o1 = newInt(vm, 1);
  Object *o2 = newInt(vm, 2);
  popFromVM(vm);
  markAll(vm);
  assert(2 == vm->stackSize);

  int counter = 0;
  ObjectLinkedList *current = vm->head;
  while (current != NULL) {
    counter += 1;
    printObject(current->object);
    current = current->next;
  }
  assert(3 == counter);

  endTestCase(vm);
}

void testMarkAndSweep() {
  printTestCase("Test mark and then sweep algorithm");
  VM *vm = newVM();
  Object *o1 = newInt(vm, 1);
  Object *o2 = newInt(vm, 2);
  popFromVM(vm);
  markAll(vm);
  assert(2 == vm->stackSize);
  int counter = 0;
  ObjectLinkedList *current = vm->head;
  while (current != NULL) {
    counter += 1;
    printObject(current->object);
    current = current->next;
  }
  assert(3 == counter);
  printf("\n");

  sweep(vm);

  counter = 0;
  current = vm->head;
  while (current != NULL) {
    counter += 1;
    printObject(current->object);
    current = current->next;
  }
  assert(2 == counter);

  endTestCase(vm);
}

void testMarkAndSweepWithArray() {
  printTestCase("Testing mark algo with an array object");
  VM *vm = newVM();
  Object *i1 = newInt(vm, 1);
  popFromVM(vm);
  Object *i2 = newInt(vm, 2);
  popFromVM(vm);
  Object *i3 = newInt(vm, 3);
  popFromVM(vm);
  Object *i4 = newInt(vm, 4);
  popFromVM(vm);
  Object *a = newArray(vm, 3, NULL);
  assert(2 == vm->stackSize);
  a->objects[0] = i1;
  a->objects[1] = i2;
  a->objects[2] = i3;
  markAll(vm);
  int counter = 0;
  ObjectLinkedList *current = vm->head;
  while (current != NULL) {
    counter += 1;
    printObject(current->object);
    current = current->next;
  }
  printf("\n");
  assert(6 == counter);
  sweep(vm);

  counter = 0;
  current = vm->head;
  while (current != NULL) {
    counter += 1;
    printObject(current->object);
    current = current->next;
  }
  assert(5 == counter);

  endTestCase(vm);
}

// TODO test cases:
// test sweep on an array that is out of scope
// test cases on cycles (both reference and array)
// full test cases with call to gc instead of markAll + sweep

int main() {
  testNewVM();
  testNewInt();
  testNewPair();
  testNewPair_Reference();
  testNewArray();
  testVmStackTests();
  testVmLinkedList();
  testFrameMarkers();
  testMark();
  testMarkAndSweep();
  testMarkAndSweepWithArray();
}
