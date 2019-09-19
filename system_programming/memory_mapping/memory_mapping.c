#include <stdlib.h>
#include <stdio.h>


int global;
int global_init = 5;
static int stat_global;
static int stat_global_initialized;

int Add(int a, int b)
{
    return a + b;
}

int main()
{
    int a = 0;
    int b = 3;
    static int stat;
    static int stat2;
    static int initializedstat = 9;
    extern int ext;
    register int reg = 7;
    volatile int vol = 8;
    int *heap = malloc(20);
    int *heap2 = malloc(40);
    int(*func)(int a, int b) = Add;
    const char *str = "my str";
    int func_return = Add(a,b);
    int c = 50;
    
    printf("in stack %lu\n", &a);
    printf("in stack %lu\n", &b);
    printf("in data bss %lu\n", &stat);
    printf("in data bss %lu\n", &stat2);
    printf("in data %lu\n", &initializedstat);
    printf("in stack extern %lu\n", &ext);
    /*printf("in stack register %p\n", &reg);*/
    printf("in heap %lu\n", heap);
    printf("in heap %lu\n", heap);
    printf("in code %lu\n", func);
    printf("in stack const %lu\n", str);
    printf("in stack %lu\n", &func_return);
    printf("in stack %lu\n", &c);
    printf("in data bss %lu\n", &global);
    printf("in data %lu\n", &global_init);
    printf("in data bss %lu\n", &stat_global);
    printf("in data %lu\n", &stat_global_initialized);
    
    return 0;
}

