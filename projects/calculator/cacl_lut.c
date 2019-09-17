#define SPACE 32

extern int InvalidErrorFunc(s_y_stacks_t stack, void *data);
extern int PushToNumStack(s_y_stacks_t stack, void *data);
extern int SpaceMoveToNext(s_y_stacks_t stack, void *data);
extern int ParenthesMoveToNext(s_y_stacks_t stack, void *data);
extern int UnaryMoveToNext(s_y_stacks_t stack, void *data);
extern int CheckStackEmpty(s_y_stacks_t stack, void *data);
extern int ParenthesFold(s_y_stacks_t stack, void *data);
extern int OperatorPush(s_y_stacks_t stack, void *data);

struct next_state_and_transition
{
	int(*action_func)(s_y_stacks_t stack, void *data);
	calculator_state_t next_state;
};

struct next_state_and_transition transition_lut[4][256];

static void InitiateWaitForNumberState()
{
	int i = 0;
	int num = 0;

	transition_lut[WAIT_FOR_NUMBER]['\0'] = {InvalidErrorFunc, ERROR};
	
	for (i = 1; i < 256; ++i)
	{
		transition_lut[WAIT_FOR_NUMBER][i].action_func = 
													  {InvalidErrorFunc, ERROR};
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

	transition_lut[WAIT_FOR_OPERATOR]['\0'] = {CheckStackEmpty, END};

	for (i = 1; i < 256; ++i)
	{
		transition_lut[WAIT_FOR_OPERATOR][i].action_func = 
													  {InvalidErrorFunc, ERROR};
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
		transition_lut[ERROR][i].action_func = {InvalidErrorFunc, ERROR};
	}
}

static void InitiateEndState()
{
	int i = 0;

	for (i = 0; i < 256; ++i)
	{
		transition_lut[END][i].action_func = {InvalidErrorFunc, ERROR};
	}
}

static void InitLuts()
{
	InitiateWaitForNumberState();
	InitiateWaitForOperatorState();
	InitiateErrorState();
	InitiateEndState();
}

