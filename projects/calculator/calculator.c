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
	int(*action_func)(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
	calculator_state_t next_state;
};

typedef double(*operation_t)(double num1, double num2);

static int InvalidErrorFunc(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int PushToNumStack(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int SpaceMoveToNext(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int ParenthesMoveToNext(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int UnaryMoveToNext(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int CheckStackEmpty(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int ParenthesFold(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static int OperatorPush(s_y_stacks_t *stack, char *data, calc_errno_t *errno);
static double Add(double num1, double num2);
static double Substract(double num1, double num2);
static double Multiply(double num1, double num2);
static double Divide(double num1, double num2);
static double Power(double num1, double num2);

/*Look Up Tables*/
static struct next_state_and_transition 
							  transition_lut[NUMBER_OF_STATES][NUMBER_OF_CHARS];
static size_t presendence_lut[NUMBER_OF_CHARS];
static operation_t do_operation_lut[2];
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
	transition_lut[WAIT_FOR_NUMBER]['+'].next_state = WAIT_FOR_NUMBER;

	transition_lut[WAIT_FOR_NUMBER]['-'].action_func = UnaryMoveToNext;
	transition_lut[WAIT_FOR_NUMBER]['-'].next_state = WAIT_FOR_NUMBER;
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

static void InitOperationLut()
{
	size_t i = 0;
	
	for (i = 0; i < 256; ++i)
	{
		operation_lut[i] = NULL;
	}
	
	operation_lut['+'] = Add;
	operation_lut['-'] = Substract;
	operation_lut['*'] = Multiply;
	operation_lut['/'] = Divide;
	operation_lut[246] = Divide;
	operation_lut['^'] = Power;
}

static void InitLuts()
{
	/*TODO function init states LUT*/
	InitiateWaitForNumberState();
	InitiateWaitForOperatorState();
	InitiateErrorState();
	InitiateEndState();

	InitOperationLut();
}

static size_t ComputePresendence(char *operator)
{
	return presendence_lut[(int)*operator];
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

double Calculator(const char *expression, calc_errno_t *errno)
{
	calculator_state_t current_state = WAIT_FOR_NUMBER;
	s_y_stacks_t *calculator = NULL;

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
		transition_lut[current_state][(int)*expression].action_func(calculator,
		(char *)expression, errno);
		current_state = 
		transition_lut[current_state][(int)*expression].next_state;
	} while (current_state != END && current_state != ERROR);
	
	return *(double *)StackPeek(calculator->numbers_stack);
}

static int InvalidErrorFunc(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	UNUSED(stack);
	UNUSED(data);
	
	*errno = CALC_ERR_INVALID_EXPRESSION;
	
	return 0;
}

static int PushToNumStack(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	double number_to_push = 0;
	char **new_position = NULL;

	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);
	
	new_position = &data;
	number_to_push = strtod(data, new_position);
	
	data = *new_position;
	*errno = CALC_SUCCESS;
	
	return StackPush(stack->numbers_stack, &number_to_push);
}

static int SpaceMoveToNext(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	UNUSED(stack);

	while (!isspace(*data))
	{
		++data;
	}

	*errno = CALC_SUCCESS;

	return 0;
}

static int ParenthesMoveToNext(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	StackPush(stack->operators_stack, data);
	++data;

	return SpaceMoveToNext(stack, data, errno);
}

static int UnaryMoveToNext(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	++data;
	
	if (!isdigit(*data))
	{
		*errno = CALC_ERR_INVALID_EXPRESSION;
		return -1;
	}

	return SpaceMoveToNext(stack, data, errno);
}

static int OperatorPush(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	char *operator_on_top = NULL;

	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);

	if (StackIsEmpty(stack->operators_stack))
	{
		StackPush(stack->operators_stack, data);
	}
	else
	{
		operator_on_top = (char *)StackPeek(stack->operators_stack);
		do_operation_lut[ComputePresendence(data) > 
					  ComputePresendence(operator_on_top)];
	}

	*errno = CALC_SUCCESS;
	
	return 0;
}

static int CheckStackEmpty(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	assert(NULL != stack);
	assert(NULL != data);
	assert(NULL != errno);
	
	*errno = StackIsEmpty(stack->operators_stack);
	UNUSED(data);
	
	return 0;
}

static int ParenthesFold(s_y_stacks_t *stack, char *data, calc_errno_t *errno)
{
	while (*(char *)StackPeek(stack->operators_stack) != '(')
	{
		double num1 = *(double *)StackPeek(stack->numbers_stack);
		double num2 = 0;
		double result = 0;
		
		StackPop(stack->numbers_stack);
		num2 = *(double *)StackPeek(stack->numbers_stack);
		StackPop(stack->numbers_stack);
		
		result = 
		operation_lut[(int)(*(char *)StackPeek(stack->operators_stack))](num1, num2);
		
		StackPop(stack->operators_stack);
		StackPush(stack->numbers_stack, &result);
	}
	
	*errno = CALC_SUCCESS;
	++data;

	return StackPop(stack->operators_stack);
}
