#include "foo.h"

int global_var = 0;

void foo()
{
    printf("this is global %d\n", global_var);
}

void foo1()
{
    printf("now i print %d", global_var + 1);
}

void foo2()
{
    printf("ramat kinon %d", global_var + 9000);
}
