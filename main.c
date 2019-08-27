#include <stdio.h>

#include "src/string.h"

char* testInitDestroy() {

    string_t string;
    string_init(&string);

    if (!string_equalsc(&string, "")) {
        return "testInitDestroy(): Equal to empty string check failed.";
    }

    string_destroy(&string);

    return NULL;

}

char* testInitString() {

    {
        string_t string;
        string_initString(&string, "s", 1);

        if (!string_equalsc(&string, "s")) {
            return "testInitString(): Equal to 's' string check failed.";
        }

        string_destroy(&string);
    }

    {
        string_t string;
        string_initString(&string, "Hello, World! How are you doing?", 32);

        if (!string_equalsc(&string, "Hello, World! How are you doing?")) {
            return "testInitString(): Equal to larger string check failed.";
        }

        string_destroy(&string);
    }

    return NULL;

}

char* testCopyString() {

    {
        string_t string;
        string_initString(&string, "s", 1);

        if (!string_equalsc(&string, "s")) {
            return "testCopyString(): Primary equal to 's' string check failed.";
        }

        string_t target;
        if (!string_copy(&string, &target)) {
            return "testCopyString(): memory error.";
        }

        if (!string_equalsc(&string, "s")) {
            return "testCopyString(): Second equal to 's' string check failed.";
        }

        if (!string_equalsc(&target, "s")) {
            return "testCopyString(): Third equal to 's' string check failed.";
        }

        if (!string_equals(&target, &string)) {
            return "testCopyString(): Fourth equal to 's' string check failed.";
        }

        if (!string_equals(&string, &target)) {
            return "testCopyString(): Fifth equal to 's' string check failed.";
        }

        if (!string_equals(&string, &string)) {
            return "testCopyString(): Six equal to 's' string check failed.";
        }

        if (!string_equals(&target, &target)) {
            return "testCopyString(): Seventh equal to 's' string check failed.";
        }

        string_destroy(&string);
    }

    return NULL;

}

int main() {

    char* (*tests[])() = {
        testInitDestroy,
        testInitString,
        testCopyString,
        NULL
    };

    puts("Starting tests...");

    size_t testsFailed = 0;
    size_t testsPassed = 0;

    char* (**currentTest)() = tests;
    char* currentTestResponse;
    while (*currentTest != NULL) {

        if ((currentTestResponse = (*currentTest)()) == NULL) {
            testsPassed++;
        }
        else {

            if (testsFailed == 0) {
                fputs("======[ FAIL ]======\n", stderr);
            }

            testsFailed++;

            fprintf(stderr, "Error: Test #%llu failed:\n", currentTest - tests);

            fprintf(stderr, "%s\n", currentTestResponse);

            fputs("===", stderr);
        }

        currentTest++;

    }

    if (testsFailed == 0) {
        puts("======[ PASS ]======");
        puts("All tests successfully passed.");
        return 0;
    }
    else {
        fputs("======[ FAIL ]======\n", stderr);
    }

    return 0;

}