#include <stdio.h>

#include "calculator.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define UNUSED(x) ((void)(x))

static void TestCalculator()
{
    const char *string = "3+4+5";
    const char *string2 = "3+4*5";
    const char *string3 = "3+4 /2* 3+  1";
    const char *string4 = "2^2";
    const char *string5 = "2-6";
    const char *string6 = "3 + (6+2*3) / 2";
    const char *string7 = "3++1--4";
    const char *string8 = "2^3^2 - 60  +4";
    const char *string9 = "2^(1+(4/+2))";
    const char *string10 = "(3+4";
    const char *string11 = "1 - - 1";
    const char *string12 = "1 / (0.1 * 5)";
    calc_errno_t errno = 0;
    double result = 0;
    size_t error_counter = 0;
    
    result = Calculator(string, &errno);
    
    if (12 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("basic operations");
    }
    
    result = Calculator(string2, &errno);

    if (23 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("basic operations");
    }

    result = Calculator(string3, &errno);

    if (10 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("basic operations");
    }
    
    result = Calculator(string4, &errno);

    if (4 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("basic operations");
    }
    
    result = Calculator(string5, &errno);

    if (-4 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("basic operations");
    }
    
    result = Calculator(string6, &errno);

    if (9 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }
    
    result = Calculator(string7, &errno);

    if (8 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }

    result = Calculator(string8, &errno);

    if (8 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }
    
    result = Calculator(string9, &errno);

    if (8 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }

    result = Calculator(string10, &errno);

    if (CALC_ERR_INVALID_EXPRESSION != errno)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }

    errno = CALC_SUCCESS;
    result = Calculator(string11, &errno);

    if (CALC_ERR_INVALID_EXPRESSION != errno)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }

    errno = CALC_SUCCESS;
    result = Calculator(string12, &errno);

    if (2 != result)
    {
        ++error_counter;
        printf("result is %f\n", result);
        FAIL("parentheses operations");
    }

    if (0 == error_counter)
    {
        PASS("Calculator");
    }
}

int main()
{
    TestCalculator();
	return 0;
}
