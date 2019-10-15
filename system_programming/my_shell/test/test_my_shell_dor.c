#include <stdio.h>
#include "my_shell.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

int main()
{
    return 0;
}
