There's 3 components to the mark and sweep garbage collector:
  Allocator, mark algorithm and sweep algorithm

There is the allocator/VM which contains:
 * A (Linked) List of all objects that have been allocated
 * A stack with all the currently reachable objects (representing the call stack)
As function calls happen, more pointers are added to the call stack. Once the function returns all the pointers are popped off the call stack (either manually or with the popFrame method)
The List of all objects keeps track of all objects that have been allocated, so that the allocator can eventually collect those objects

Example of call stack

```
/ a
/ b
/ func1()
/ y
/ x
/ main()
/ ****** /
```
After exiting func1(), the call stack would look like
```
/ y
/ x
/ main()
/ ****** /
```

In both cases, the list of all objects would looks like
```
[FrameMarker(main), x, y, FrameMarker(func1), a, b]
```

The mark algorithm would go through each element in the current stack and marks them as reachable.
In addition, if a variable on the stack has a reference to another variable, the mark algorithm will follow that reference (through DFS, with cycle detection) and mark that variable too.
In the above example (after exiting func1() ), the list of all objects after marking will look like:
```
[ *FrameMarker(main), *x, *y, FrameMarker(func1), a, b ]
```

The sweep algorithm then walks through the linked list of all objects.
It frees objects if they weren't marked as reachable, or unmarks the objects if they were reachable. After the sweep algorithm runs, the list of all objects would look like:

```
[ FrameMarker(main), x, y ]
```
