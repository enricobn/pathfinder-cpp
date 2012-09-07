#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef GLOBALS_H_
#define GLOBALS_H_

typedef enum {FALSE, TRUE} boolean;

#define ERROR(...) fprintf(stderr, "%s(%d) ", __FILE__, __LINE__);fprintf(stderr, __VA_ARGS__);fprintf(stderr, "\n")

#define NEW_POINTER(TYPE) (TYPE *) malloc(sizeof(TYPE))

#endif