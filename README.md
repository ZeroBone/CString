# Features
* O(1) length lookup: no `strlen` O(n) frustrating complexity.
* Helper methods for integration with standard C strings (char arrays).
* STL-compatible: The null-terminator is maintained.
* Read-only buffer access.
* Small strings (by default <= 11 bytes) are stored in the descriptor, no additional memory is allocated on the heap.
  When the string grows it gets copied to the heap automatically.
  When the string shrinks it gets copied to the descriptor to minimize the amount of allocation on the heap.
* No implicit malloc and free for the descriptor: You can choose where to allocate it - on the stack, on the heap or even in static memory.
* Readable code with assertions.
* Memory safe: all memory errors are reported to the callee and the state doesn't break.
# Memory usage
The string descriptor requires `24` bytes in a 64-bit environment (with `sizeof(void*) == 8`)
and `16` bytes in a 32-bit environment (with `sizeof(void*) == 4`). The size of the descriptor cannot be changed.

This memory usage can be adjusted significantly by customizing the `STRING_SHORT_STRING_LENGTH` macro.    
# Representation
The string is represented as following:
```c
struct string_s {
    size_t length;
    union {
        struct {
            char str[STRING_SHORT_STRING_LENGTH]; // string with a null-terminator
        } shortStr; // used if length < STRING_SHORT_STRING_LENGTH
        struct { // this struct is used for long strings
            char* str; // pointer to heap-allocated region, null-terminator maintained
            size_t allocated; // the amount of bytes allocated.
        } str; // used if length >= STRING_SHORT_STRING_LENGTH
    } types;
};
```
# License
Copyright (c) 2019 Alexander Mayorov.

This project is licensed under the MIT License.
Please leave a copyright notice if you use this software.
For more details see the LICENCE file.