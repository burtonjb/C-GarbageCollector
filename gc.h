#ifndef GC_H_GUARD
#define GC_H_GUARD

#define MAX_STACK_SIZE 256
#define FALSE 0
#define TRUE 1

/* Enum to switch on the type for the object */
typedef enum {
  VALUE,          // A single value. This will just be an integer for now
  REFERENCE_PAIR, // A Pair of reference (to objects)
  ARRAY,        // An array of object references. The array will have to be null
                // terminated just like C strings.
  FRAME_MARKER, // A special object to denote the start of a stack frame
  START_OF_STACK // A special object to denote that this is the start of the
                 // stack for the VM
} ObjectType;

typedef struct object {
  ObjectType objectType;
  unsigned char marked;
  union {
    // Value object (just an int)
    int value;

    // Reference pair object
    struct {
      struct object *head;
      struct object *tail;
    };

    // Array
    struct object **array;
  };
} Object;

void printObject(Object *object);

/* LinkedList implementation for all the objects that have been created in the
 * VM */
typedef struct objectLinkedList {
  Object *object;
  struct objectLinkedList *next;
} ObjectLinkedList;

ObjectLinkedList *appendObject(ObjectLinkedList *tail, Object *object);

/* VM object to keep track of all objects in scope and also all objects */
typedef struct {
  int stackSize;                 // current size of the stack
  Object *stack[MAX_STACK_SIZE]; // stack to keep track of all objects in scope
  ObjectLinkedList *tail; // linked list to keep track of all objects. Currently
                          // the end of the list
  ObjectLinkedList *head; // Linked list to keep track of all objects. Start of
                          // the LL.
} VM;

VM *newVM();
void pushToVM(VM *vm, Object *object);
Object *popFromVM(VM *vm);

Object *newInt(VM *vm, int value);
Object *newReferencePair(VM *vm, Object *head, Object *tail);

/* Methods to simulate calling a function or returning from a function */
void pushFrame(VM *vm); // Adds a frame object to the stack
void popFrame(VM *vm); // Removes items from the stack until a frame object is
                       // reached (and pops off that frame object)

void mark(Object *object);
void markAll(VM *vm);
void sweep(VM *vm);
void gc(VM* vm);

#endif
