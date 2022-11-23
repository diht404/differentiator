#ifndef RECURSIVE_DESCENT__RECURSIVE_DESCENT_H
#define RECURSIVE_DESCENT__RECURSIVE_DESCENT_H

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include "ctype.h"
#include "cmath"
#include "tree.h"

Node *recursiveDescent(const char **program);

Node *getAddSub(const char **program);

Node *getMulDiv(const char **program);

Node *getPow(const char **program);

Node *getLog(const char **program);

Node *getSin(const char **program);

Node *getCos(const char **program);

Node *getBrackets(const char **program);

Node *getValue(const char **program);

Node *getVariable(const char **program);

#define ASSERT_OK(value, error, ...)                                \
        if (!(value))                                               \
        {                                                           \
            fprintf(stderr, "Error occured: " error, ##__VA_ARGS__);\
            abort();                                                \
        }

#endif //RECURSIVE_DESCENT__RECURSIVE_DESCENT_H