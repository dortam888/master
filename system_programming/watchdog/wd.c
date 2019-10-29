#define _POSIX_C_SOURCE 1

#include "watchdog.h"


int main(int argc, char *argv[])
{
    setenv("wd_flag", "1", 0);
    
    return WDMakeMeImmortal(argv, interval);
}
