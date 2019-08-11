#include "circular_buffer.h"
#include <stdio.h>

#define PASS(function) printf("\033[0;32m%s\033[0m\n", #function)
#define FAIL(function) printf("\033[0;31m%s\033[0m\n in line %d", #function, __LINE__)

int main()
{
    return 0;
}
