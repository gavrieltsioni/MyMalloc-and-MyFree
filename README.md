# MyMalloc-and-MyFree

An implementation of malloc() and free() in C that detects various user errors including:

* Double freeing of a pointer.

* Freeing of an address that was not previously allocated but is in dynamic memory.

* Freeing of an address that is not in dynamic memory.

* Saturation of dynamic memory.

* Memory fragmentation
