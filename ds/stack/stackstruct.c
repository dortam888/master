struct stack_t
{
	void *base;
	void *current;
	void *end;
	size_t size_of_element;
}
