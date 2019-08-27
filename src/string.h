#ifndef CSTRING_STRING_H
#define CSTRING_STRING_H

#define STRING_NO_DEBUG

#include <stdbool.h>
#include <stdlib.h>

#define STRING_SHORT_STRING_LENGTH 12 // should be multiple of word size for space efficiency, not less than 1

typedef struct {
    size_t length;
    union {
        struct {
            char str[STRING_SHORT_STRING_LENGTH];
        } shortStr;
        struct {
            char* str;
            size_t allocated;
        } str;
    } types;
} string_t;

#ifdef STRING_NO_DEBUG
#undef STRING_NO_DEBUG
#define STRING_DEBUG(str, ...)
#else
#define STRING_DEBUG(str, ...) \
    printf("[STRING]: "); \
    printf(str, __VA_ARGS__); \
    puts("");
#endif

void string_init(string_t* const string);
bool string_initString(string_t* const string, const char* const cString, const size_t cStrLength);
bool string_copy(const string_t* const from, string_t* const to);
bool string_concat(string_t* const string, const string_t* const other);
bool string_concatChar(string_t* const string, const char c);
const char* string_getBuffer(const string_t* const string);
const unsigned char* string_getBufferUnsigned(const string_t* const string);
bool string_equals(const string_t* const str1, const string_t* const str2);
bool string_equalsc(const string_t* const string, const char* const cstring);
bool string_equalscl(const string_t* const string, const char* const cstring, const size_t cstringLength);
bool string_startsWithChar(const string_t* const string, const char c);
void string_destroy(string_t* const string);

#endif //CSTRING_STRING_H