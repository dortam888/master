#include <stdlib.h> /*strtod, malloc, free*/
#include <assert.h> /*assert*/
#include <string.h> /*strlen*/
#include <ctype.h> /* isspace */
#include <math.h> /* pow */

#include "stack.h"
#include "calculator.h"

#define NUMBER_OF_CHARS 256
#define NUMBER_OF_STATES 4
#define SPACE 32
#define UNUSED(x) ((void)(x))

typedef enum calculator_states 
{
	WAIT_FOR_NUMBER,
	WAIT_FOR_OPERATOR,
	END,
	ERROR
}calculator_state_t;

typedef struct shunting_yard_stacks
{
	stack_t *numbers_stack;
	stack_t *operators_stack;
}s_y_stacks_t;

struct next_state_and_transition
{
	char*(*action_func)(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
	calculator_state_t next_state;
};

typedef double(*operation_t)(double num1, double num2);
typedef void(*do_operation_t)(s_y_stacks_t *stack, char *data);
typedef void(*sign_t)(char *data);

static char *InvalidErrorFunc(s_y_stacks_t *stack, char *data, 
                              calc_errno_t *errno);
static char *PushToNumStack(s_y_stacks_t *stack, char *data, 
                            calc_errno_t *errno);
static char *SpaceMoveToNext(s_y_stacks_t *stack, char *data, 
                             calc_errno_t *errno);
static char *ParenthesMoveToNext(s_y_stacks_t *stack, char *data, 
                                 calc_errno_t *errno);
static char *UnaryMoveToNext(s_y_stacks_t *stack, char *data, 
                             calc_errno_t *errno);
static char *CheckStackEmpty(s_y_stacks_t *stack, char *data, 
                             calc_errno_t *errno);
static char *ParenthesFold(s_y_stacks_t *stack, char *data, 
                           calc_errno_t *errno);
static char *OperatorPush(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static void PushOperator(s_y_stacks_t *stack, char *data);
static void FoldAndPush(s_y_stacks_t *stack, char *data);
static void CheckPresendence(s_y_stacks_t *stack, char *data);
static double Add(double num1, double num2);
static double Substract(double num1, double num2);
static double Multiply(double num1, double num2);
static double Divide(double num1, double num2);
static double Power(double num1, double num2);

/*Look Up Tables*/
static struct next_state_and_transition 
							  transition_lut[NUMBER_OF_STATES][NUMBER_OF_CHARS];
static size_t presendence_lut[NUMBER_OF_CHARS];
static do_operation_t do_operation_lut[2] = {FoldAndPush, PushOperator};
static do_operation_t is_stack_empty_lut[2] = {CheckPresendence, PushOperator};
static operation_t operation_lut[NUMBER_OF_CHARS];

/*Operations*/
static double Add(double num1, double num2)
{
	return num2 + num1;
}

static double Substract(double num1, double num2)
{
	return num2 - num1;
}

static double Multiply(double num1, double num2)
{
	return num2 * num1;
}

static double Divide(double num1, double num2)
{
    if (0 == num1)
    {
        return 0xDEADDEAD;
    }

	return num2 / num1;
}

static double Power(double num1, double num2)
{
	return pow(num2, num1);
}

/*Init Luts*/
static void InitiateWaitForNumberState()
{
	int i = 0;
	int num = 0;

	transition_lut[WAIT_FOR_NUMBER]['\0'].action_func = InvalidErrorFunc;
	transition_lut[WAIT_FOR_NUMBER]['\0'].next_state = ERROR;

	for (i = 1; i < 256; ++i)
	{
		transition_lut[WAIT_FOR_NUMBER][i].action_func = InvalidErrorFunc;
		transition_lut[WAIT_FOR_NUMBER][i].next_state = ERROR;
	}
	
	for (num = '0'; num <= '9'; ++num)
	{
		transition_lut[WAIT_FOR_NUMBER][num].action_func = PushToNumStack;
		transition_lut[WAIT_FOR_NUMBER][num].next_state = WAIT_FOR_OPERATOR;
	}

	transition_lut[WAIT_FOR_NUMBER][SPACE].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_NUMBER][SPACE].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_NUMBER]['('].action_func = ParenthesMoveToNext;
	transition_lut[WAIT_FOR_NUMBER]['('].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_NUMBER]['+'].action_func = UnaryMoveToNext;
	transition_lut[WAIT_FOR_NUMBER]['+'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_NUMBER]['-'].action_func = UnaryMoveToNext;
	transition_lut[WAIT_FOR_NUMBER]['-'].next_state = WAIT_FOR_OPERATOR;
}

static void InitiateWaitForOperatorState()
{
	int i = 0;

	transition_lut[WAIT_FOR_OPERATOR]['\0'].action_func = CheckStackEmpty;
	transition_lut[WAIT_FOR_OPERATOR]['\0'].next_state = END;

	for (i = 1; i < 256; ++i)
	{
		transition_lut[WAIT_FOR_OPERATOR][i].action_func = InvalidErrorFunc;
		transition_lut[WAIT_FOR_OPERATOR][i].next_state = ERROR;
	}

	transition_lut[WAIT_FOR_OPERATOR][SPACE].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_OPERATOR][SPACE].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR]['\t'].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_OPERATOR]['\t'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR]['\n'].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_OPERATOR]['\n'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR]['\v'].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_OPERATOR]['\v'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR]['\f'].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_OPERATOR]['\f'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR]['\r'].action_func = SpaceMoveToNext;
	transition_lut[WAIT_FOR_OPERATOR]['\r'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR][')'].action_func = ParenthesFold;
	transition_lut[WAIT_FOR_OPERATOR][')'].next_state = WAIT_FOR_OPERATOR;

	transition_lut[WAIT_FOR_OPERATOR]['*'].action_func = OperatorPush;
	transition_lut[WAIT_FOR_OPERATOR]['*'].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_OPERATOR]['+'].action_func = OperatorPush;
	transition_lut[WAIT_FOR_OPERATOR]['+'].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_OPERATOR]['-'].action_func = OperatorPush;
	transition_lut[WAIT_FOR_OPERATOR]['-'].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_OPERATOR]['/'].action_func = OperatorPush;
	transition_lut[WAIT_FOR_OPERATOR]['/'].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_OPERATOR][246].action_func = OperatorPush;
	transition_lut[WAIT_FOR_OPERATOR][246].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_OPERATOR]['^'].action_func = OperatorPush;
	transition_lut[WAIT_FOR_OPERATOR]['^'].next_state = WAIT_FOR_NUMBER;
}

static void InitiateErrorState()
{
	int i = 0;

	for (i = 0; i < 256; ++i)
	{
		transition_lut[ERROR][i].action_func = InvalidErrorFunc;
		transition_lut[ERROR][i].next_state = ERROR;
	}
}

static void InitiateEndState()
{
	int i = 0;

	for (i = 0; i < 256; ++i)
	{
		transition_lut[END][i].action_func = InvalidErrorFunc;
		transition_lut[END][i].next_state = ERROR;
	}
}

static void InitOperationAndPresendenceLut()
{
	size_t i = 0;
	
	for (i = 0; i < 256; ++i)
	{
		operation_lut[i] = NULL;
		presendence_lut[i] = 0;
	}
	
	operation_lut['+'] = Add;
	presendence_lut['+'] = 1;
	operation_lut['-'] = Substract;
	presendence_lut['-'] = 1;
	operation_lut['*'] = Multiply;
	presendence_lut['*'] = 2;
	operation_lut['/'] = Divide;
	presendence_lut['/'] = 2;
	operation_lut[246] = Divide;
	presendence_lut[246] = 2;
	operation_lut['^'] = Power;
	presendence_lut['^'] = 3;
}

static void InitLuts()
{
	/*TODO function init states LUT*/
	InitiateWaitForNumberState();
	InitiateWaitForOperatorState();
	InitiateErrorState();
	InitiateEndState();

	InitOperationAndPresendenceLut();
}

static size_t ComputePresendence(unsigned char *operator)
{
	return presendence_lut[(int)(*operator)];
}

static s_y_stacks_t *InitShuntingYard(size_t stack_size)
{
	s_y_stacks_t *init_calculator = 
								(s_y_stacks_t *)malloc(sizeof(s_y_stacks_t));
	
	init_calculator->numbers_stack = StackCreate(stack_size, sizeof(double));
	if (NULL == init_calculator->numbers_stack)
	{
		free(init_calculator); init_calculator = NULL;
		return NULL;
	}

	init_calculator->operators_stack = StackCreate(stack_size, sizeof(char));
	if (NULL == init_calculator->operators_stack)
	{
		free(init_calculator); init_calculator = NULL;
		StackDestroy(init_calculator->numbers_stack);
		init_calculator->numbers_stack = NULL;
		return NULL;
	}

	return init_calculator;
}

static void DestroyCalculator(s_y_stacks_t *calculator)
{
    StackDestroy(calculator->numbers_stack); 
    calculator->numbers_stack = NULL;
    StackDestroy(calculator->operators_stack); 
    calculator->operators_stack = NULL;
    
    free(calculator); calculator = NULL;
}

double Calculator(const char *expression, calc_errno_t *errno)
{
	calculator_state_t current_state = WAIT_FOR_NUMBER;
	s_y_stacks_t *calculator = NULL;
	char *data = (char *)expression;
	double result = 0;

	assert(NULL != expression);
	assert(NULL != errno);

	calculator = InitShuntingYard(strlen(expression));
	if (NULL == calculator)
	{
		*errno = CALC_FAILED_ALOC;
		return (double)0xDEADA110C;
	}

	InitLuts();

	do
	{
	    char *prev_data = data;
		data = transition_lut[current_state][(int)*data].action_func(calculator,
		data, errno);
		current_state = 
		transition_lut[current_state][(int)*prev_data].next_state;
	} while (current_state != END && current_state != ERROR);
	
	result = *(double *)StackPeek(calculator->numbers_stack);

	DestroyCalculator(calculator);

	return result;
}

static char *InvalidErrorFunc(s_y_stacks_t *stack, char *data, 
                              calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	UNUSED(stack);
	UNUSED(data);
	
	*errno = CALC_ERR_INVALID_EXPRESSION;
	
	return NULL;
}

static char *PushToNumStack(s_y_stacks_t *stack, char *data, 
                            calc_errno_t *errno)
{
	double number_to_push = 0;
	char **new_position = NULL;

	assert(NULL != stack);1/0 in c
	assert(NULL != data);
	assert(NULL != errno);
	
	new_position = &data;
	number_to_push = strtod(data, new_position);
	
	*errno = CALC_SUCCESS;
	
	StackPush(stack->numbers_stack, &number_to_push);
	
	return data;
}

static char *SpaceMoveToNext(s_y_stacks_t *stack, char *data, 
                             calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	UNUSED(stack);

	while (isspace(*data))
	{
		++data;
	}

	*errno = CALC_SUCCESS;

	return data;
}

static char *ParenthesMoveToNext(s_y_stacks_t *stack, char *data, 
                                 calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	StackPush(stack->operators_stack, data);
	++data;

	return SpaceMoveToNext(stack, data, errno);
}

static char *UnaryMoveToNext(s_y_stacks_t *stack, char *data, 
                             calc_errno_t *errno)
{
    char ch = 1;
    char *error_data = &ch;
    char **new_position = NULL;
    double number_to_push = 0;

	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	if (!isdigit(*(data+1)))
	{
		*errno = CALC_ERR_INVALID_EXPRESSION;
		return error_data;
	}

    new_position = &data;
	number_to_push = strtod(data, new_position);
    StackPush(stack->numbers_stack, &number_to_push);

    errno = CALC_SUCCESS;

	return data;
}

static void PushOperator(s_y_stacks_t *stack, char *data)
{
    assert(NULL != stack);
    assert(NULL != data);

    StackPush(stack->operators_stack, data);
}

static void FoldAndPush(s_y_stacks_t *stack, char *data)
{
    double num1 = 0;
    double num2 = 0;
    double result = 0;

    assert(NULL != stack);
    assert(NULL != data);

    num1 = *(double *)StackPeek(stack->numbers_stack);
    StackPop(stack->numbers_stack);
    num2 = *(double *)StackPeek(stack->numbers_stack);
    StackPop(stack->numbers_stack);
    result = 
    operation_lut[(int)(*(unsigned char *)StackPeek(stack->operators_stack))]
                                                                (num1, num2);
    StackPop(stack->operators_stack);
    StackPush(stack->numbers_stack, &result);
    
    is_stack_empty_lut[StackIsEmpty(stack->operators_stack)](stack, data);
}

static void CheckPresendence(s_y_stacks_t *stack, char *data)
{
    unsigned char *operator_on_top = NULL;

    assert(NULL != stack);
    assert(NULL != data);

    operator_on_top = (unsigned char *)StackPeek(stack->operators_stack);
    do_operation_lut[ComputePresendence((unsigned char *)data) > 
                     ComputePresendence(operator_on_top)](stack, data);
}

static char *OperatorPush(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

    is_stack_empty_lut[StackIsEmpty(stack->operators_stack)](stack, data);

	*errno = CALC_SUCCESS;
	++data;
	
	return data;
}

static char *CheckStackEmpty(s_y_stacks_t *stack, char *data, 
                             calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);
	
	if (StackSize(stack->operators_stack) == StackSize(stack->numbers_stack))
	{
	    double err_num = 0xDEADDEADDEAD;
	
	    while (!StackIsEmpty(stack->numbers_stack))
	    {
	        StackPop(stack->numbers_stack);
	    }
	    
	    StackPush(stack->numbers_stack, &err_num);
	    *errno = CALC_ERR_INVALID_EXPRESSION;
	}
	else
	{
		is_stack_empty_lut[StackIsEmpty(stack->operators_stack)](stack, data);
	    *errno = CALC_SUCCESS;
	}

	
	return NULL;
}

static char *ParenthesFold(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	while (*(unsigned char *)StackPeek(stack->operators_stack) != '(')
	{
		double num1 = *(double *)StackPeek(stack->numbers_stack);
		double num2 = 0;
		double result = 0;
		
		StackPop(stack->numbers_stack);
		num2 = *(double *)StackPeek(stack->numbers_stack);
		StackPop(stack->numbers_stack);
		
		result = 
		operation_lut[(int)*(unsigned char *)StackPeek(stack->operators_stack)]
		(num1, num2);
		
		StackPop(stack->operators_stack);
		StackPush(stack->numbers_stack, &result);
	}

	*errno = CALC_SUCCESS;
	++data;

	StackPop(stack->operators_stack);

	return data;
}

