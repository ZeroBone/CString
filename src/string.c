#include "string.h"
#include <string.h>
#include "assert.h"

#define STRING_IS_SHORT(str) ((str).length < STRING_SHORT_STRING_LENGTH)
#define STRING_LENGTH_IS_SHORT(length) (length < STRING_SHORT_STRING_LENGTH)

#define STRING_RESIZE_OVERHEAD 16

void string_init(string_t* const string) {

    string->length = 0;

    string->types.shortStr.str[0] = '\0';

}

bool string_initString(string_t* const string, const char* const cString, const size_t cStrLength) {

    string->length = cStrLength;

    if (STRING_LENGTH_IS_SHORT(cStrLength)) {

        memcpy(string->types.shortStr.str, cString, cStrLength + 1);

        return true;

    }

    char* newString = (char*)malloc(cStrLength + 1);

    if (newString == NULL) {
        return false;
    }

    string->types.str.str = newString;

    memcpy(string->types.str.str, cString, cStrLength + 1);

    return true;

}

bool string_copy(const string_t* const from, string_t* const to) {

    to->length = from->length;

    if (STRING_IS_SHORT(*from)) {

        memcpy(to->types.shortStr.str, from->types.shortStr.str, from->length + 1);

        return true;

    }

    // regular string

    to->types.str.allocated = from->types.str.allocated;

    char* newString = (char*)malloc(from->types.str.allocated);

    if (newString == NULL) {
        return false;
    }

    memcpy(newString, from->types.str.str, from->length + 1);

    to->types.str.str = newString;

    return true;

}

bool string_concat(string_t* const string, const string_t* const other) {

    if (STRING_LENGTH_IS_SHORT(string->length + other->length)) {

        // if the sum corresponds to a short string, both strings are short

        memcpy(string->types.shortStr.str + string->length, other->types.shortStr.str, other->length + 1);

        string->length += other->length;

        return true;

    }

    if (STRING_IS_SHORT(*string) && STRING_IS_SHORT(*other)) {

        // both strings are short, but the concatenated string is not short

        char* newString = (char*)malloc(string->length + other->length + 1 + STRING_RESIZE_OVERHEAD);

        if (newString == NULL) {
            return false;
        }

        // copy the first part
        memcpy(newString, string->types.shortStr.str, string->length);

        // copy the second part
        memcpy(newString + string->length, other->types.shortStr.str, other->length + 1);

        string->types.str.str = newString;

        string->length += other->length;

        string->types.str.allocated = string->length + 1 + STRING_RESIZE_OVERHEAD;

        return true;

    }

    if (STRING_IS_SHORT(*string)) {
        // at this point we know for sure the other string is long

        char* newString = (char*)malloc(string->length + other->length + 1 + STRING_RESIZE_OVERHEAD);

        if (newString == NULL) {
            return false;
        }

        // copy short string
        memcpy(newString, string->types.shortStr.str, string->length);

        // copy long concatenated string
        memcpy(newString + string->length, other->types.str.str, other->length + 1);

        string->types.str.str = newString;

        string->length += other->length;

        string->types.str.allocated = string->length + 1 + STRING_RESIZE_OVERHEAD;

        return true;

    }

    // at this point we know for sure string is long

    // the concatenated string may fit at the end of the allocated region,
    // especially if the concatenated string is short

    if (string->length + other->length < string->types.str.allocated) { // less because of the null-terminator
        // no need to allocate new memory

        memcpy(
            string->types.str.str + string->length,
            STRING_IS_SHORT(*other) ? other->types.shortStr.str : other->types.str.str,
            other->length
        );

        string->length += other->length;

        return true;

    }

    char* newString = (char*)realloc(string->types.str.str, string->length + other->length + 1 + STRING_RESIZE_OVERHEAD);

    if (newString == NULL) {
        return false;
    }

    // the first part is already copied by realloc, copy the concatenated string
    memcpy(
        newString + string->length,
        STRING_IS_SHORT(*other) ? other->types.shortStr.str : other->types.str.str,
        other->length
    );

    string->types.str.str = newString;

    string->length += other->length;

    string->types.str.allocated = string->length + 1 + STRING_RESIZE_OVERHEAD;

    return true;

}

bool string_concatChar(string_t* const string, const char c) {

    if (STRING_LENGTH_IS_SHORT(string->length + 1)) {
        // string is short

        STRING_DEBUG("[ConcatChar]: String is short and the new string is also short (%zu => %zu).", string->length, string->length + 1);

        string->types.shortStr.str[string->length++] = c;

        string->types.shortStr.str[string->length] = '\0';

        return true;

    }

    if (STRING_IS_SHORT(*string)) {

        STRING_DEBUG("[ConcatChar]: String is short, but the new string is not short (%zu => %zu).", string->length, string->length + 1);

        // string is short, but the string with the concatenated char is not short

        char* newString = (char*)malloc(string->length + 2 + STRING_RESIZE_OVERHEAD);

        if (newString == NULL) {
            return false;
        }

        // copy the first part
        memcpy(newString, string->types.shortStr.str, string->length);

        string->types.str.allocated = string->length + 2 + STRING_RESIZE_OVERHEAD;

        newString[string->length++] = c;
        newString[string->length] = '\0';

        STRING_DEBUG("[ConcatChar]: Put the char and the null terminator (%s).", newString);

        string->types.str.str = newString;

        assert(!STRING_IS_SHORT(*string));
        assert(!STRING_LENGTH_IS_SHORT(string->length));
        assert(string_getBuffer(string) == newString);

        STRING_DEBUG("[ConcatChar]: Finished concatinating (%s).", string_getBuffer(string));

        return true;

    }

    assert(!STRING_IS_SHORT(*string));
    assert(!STRING_LENGTH_IS_SHORT(string->length));

    if (string->length + 1 < string->types.str.allocated) { // less because of the null-terminator
        // no need to allocate new memory

        STRING_DEBUG("[ConcatChar]: String is long, but there is enouph space to expand (%zu => %zu).", string->length, string->length + 1);

        string->types.str.str[string->length++] = c;
        string->types.str.str[string->length] = '\0';

        return true;

    }

    STRING_DEBUG("[ConcatChar]: String is long, reallocating memory (%zu => %zu).", string->length, string->length + 1);

    char* newString = (char*)realloc(string->types.str.str, string->length + 2 + STRING_RESIZE_OVERHEAD);

    if (newString == NULL) {
        return false;
    }

    // the first part is already copied by realloc, copy the concatenated string
    newString[string->length++] = c;
    newString[string->length] = '\0';

    string->types.str.str = newString;

    string->types.str.allocated = string->length + 1 + STRING_RESIZE_OVERHEAD; // +1 and not +2 because we incremented the length

    return true;

}

const char* string_getBuffer(const string_t* const string) {

    return STRING_IS_SHORT(*string) ? string->types.shortStr.str : string->types.str.str;

}

const unsigned char* string_getBufferUnsigned(const string_t* const string) {

    return STRING_IS_SHORT(*string) ? (unsigned char*)string->types.shortStr.str : (unsigned char*)string->types.str.str;

}

bool string_equals(const string_t* const str1, const string_t* const str2) {

    if (str1->length != str2->length) {
        return false;
    }

    if (STRING_IS_SHORT(*str1)) {

        size_t i;
        for (i = 0; i != str1->length; i++) {

            if (str1->types.shortStr.str[i] != str2->types.shortStr.str[i]) {
                return false;
            }

        }

        return true;

    }

    size_t i;
    for (i = 0; i != str1->length; i++) {

        if (str1->types.str.str[i] != str2->types.str.str[i]) {
            return false;
        }

    }

    return true;

}

/**
 * Compares the string with the C-style string.
 * This method should be used when the calling function doesn't know about the length of the C-string.
 * @param string the string.
 * @param cstring C-style string.
 * @return true if strings match, false otherwise.
 */
bool string_equalsc(const string_t* const string, const char* const cstring) {

    if (STRING_IS_SHORT(*string)) {

        size_t i;
        for (i = 0; i != string->length; i++) {

            if (cstring[i] == '\0') {
                return false;
            }

            if (string->types.shortStr.str[i] != cstring[i]) {
                return false;
            }

        }

        return cstring[i] == '\0'; // make sure the cstring ends at this point

    }

    // string is not short

    size_t i;
    for (i = 0; i != string->length; i++) {

        if (cstring[i] == '\0') {
            return false;
        }

        if (string->types.str.str[i] != cstring[i]) {
            return false;
        }

    }

    return cstring[i] == '\0'; // make sure the cstring ends at this point

}

bool string_equalscl(const string_t* const string, const char* const cstring, const size_t cstringLength) {

    if (string->length != cstringLength) {
        return false;
    }

    return memcmp(string_getBuffer(string), cstring, cstringLength) == 0;

}

bool string_startsWithChar(const string_t* const string, const char c) {

    if (string->length == 0) {
        return false;
    }

    return STRING_IS_SHORT(*string) ? string->types.shortStr.str[0] == c : string->types.str.str[0] == c;

}

void string_destroy(string_t* const string) {

    if (STRING_IS_SHORT(*string)) {
        return;
    }

    free(string->types.str.str);

}

#undef STRING_IS_SHORT
#undef STRING_LENGTH_IS_SHORT