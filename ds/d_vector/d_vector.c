#include "d_vector.h" /* d_vector_t */
#include <stdlib.h> /* malloc free */
#include <string.h> /* memcpy */

#define DOUBLE_SIZE(size) size * 2
#define QUARTER_SIZE(size) size / 4

enum function_status {OK = 0, FAILED_TO_ALLOCATE_MEMORY = -1, 
					  SMALLER_THAN_CURRENT_SIZE = 1, VECTOR_EMPTY = 2};

struct d_vector
{
	void *start;
	void *end;
	void *current;
	size_t size_of_element;
}


d_vector_t *DVectorCreate(size_t size_of_element, size_t capacity)
{
	size_t = vector_length = capacity * size_of_element;
	d_vector_t *d_vector = malloc(sizeof(struct d_vector));

	if (NULL != d_vector)
	{
		d_vector -> start = malloc(vector_length);

		if (NULL != d_vector -> start)
		{
			d_vector -> current = d_vector -> start;
			d_vector -> end = (char *)d_vector -> start + vector_length;
			d_vector -> size_of_element = size_of_element;
		}
		else
		{
			free(d_vector);
			d_vector = NULL
		}
	}

	return d_vector;
}


void DVectorDestroy(d_vector_t *d_vector)
{
	free(d_vector -> start);
	d_vector -> start = NULL;
	d_vector -> current = NULL;
	d_vector -> end = NULL;

	free(d_vector);
}


size_t DVectorSize(const vector_t *d_vector)
{
	return ((size_t)d_vector -> current - (size_t)d_vector -> start) / 
													d_vector -> size_of_element;
}


int DVectorIsEmpty(const vector_t *d_vector)
{
	return (d_vector -> current == d_vector -> start);
}


static void Push(vector_t *d_vector, void *data)
{
	assert(d_vector != NULL && data != NULL);

	memcpy(d_vector -> current, data, d_vector -> size_of_element);
	d_vector -> current = (char *)d_vector -> current + 
													d_vector -> size_of_element;
}


int DVectorPushBack(vector_t *d_vector, const void *data)
{
	void *new_vector_start = NULL;
	size_t size_of_d_vector = DVectorSize(d_vector);

	if (DVectorCapacity(d_vector) == size_of_d_vector)
	{
		new_vector_start = realloc(d_vector -> start, 
												 DOUBLE_SIZE(size_of_d_vector));

		if (NULL == new_vector_start)
		{
			return FAILED_TO_ALLOCATE_MEMORY;
		}

		Push(new_vector_start, data);
		d_vector -> start = new_vector_start;
	}

	Push(d_vector -> current, data);

	return OK;
}


int DVectorPopBack(vector_t *d_vector)
{

}


void *DVectorGetItemAddress(const vector_t *d_vector, size_t index)
{
	return (char *)d_vector -> start + index * d_vector -> size_of_element;
}


size_t DVectorCapacity(const vector_t *d_vector)
{
	return (size_t)d_vector -> end - (size_t)d_vector -> start;
}


int DVectorReserve(vector_t *d_vector, size_t number_of_elements)
{
	void *new_vector_start = NULL
	ptrdiff_t current_to_start = (char *)d_vector -> current - 
								 (char *)d_vector -> start;

	if (number_of_elements < DVectorSize(d_vector))
	{
		return SMALLER_THAN_CURRENT_SIZE;
	}

	new_vector_start = realloc(d_vector -> start,
						number_of_elements * d_vector -> size_of_element);

	if (NULL == new_vector_start)
	{
		return FAILED_TO_ALLOCATE_MEMORY;
	}

	d_vector -> start = new_vector_start;
	d_vector -> current = (char *)d_vector -> start + current_to_start;

	return OK;
}

