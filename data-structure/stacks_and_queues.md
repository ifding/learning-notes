# Stacks and queues

## Containers

* abstract data structure that permits storage and retrieval of data items independent of content
* they're distinguished by the retrieval order they support
* in stacks and queues this retrieval order depends on the insertion order

## Stacks

* support retrieval by last-in, first-out (LIFO) order
* push - insert item at the top of the stack
* pop - return (and remove) the top item of the stack

## Queues

* support retrieval in first in, first out (FIFO) order
* minimize the maximum time spent waiting
* (average waiting time will be the same regardless of whether FIFO or LIFO is used)
* enqueue - insert an item at the back of the queue
* dequeue - return (and remove) the front item from the queue

Both stacks and queues can be effectively implemented using either arrays or linked lists.
