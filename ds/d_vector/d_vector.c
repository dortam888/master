/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: 15/08/2019
**** Reviewer : Daniel
*******************************************************************************/

#include <stdlib.h> /* malloc free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#include "d_vector.h" /* d_vector_t */

#define GROWTH_FACTOR (2)
#define SHRINK_FACTOR (4)

enum function_status 
{
	SUCCESS = 0,
	FAILED_TO_ALLOCATE_MEMORY = -1
};

struct d_vector
{
	void *start; /* pointer to start of the vector */
	void *end; /* pointer to end of the vector */
	void *current; /* pointer to current position in vector */
	size_t size_of_element;
};

static int DVectorResize(d_vector_t *d_vector, size_t number_of_elements)
{
	int function_status = SUCCESS;
	void *new_vector_start = NULL;
	ptrdiff_t current_to_start = (char *)d_vector->current -
								 (char *)d_vector->start;
	size_t new_vector_capacity = number_of_elements * d_vector->size_of_element;

	assert(NULL != d_vector);

	new_vector_start = realloc(d_vector->start, new_vector_capacity);
	if (NULL == new_vector_start)
	{
		return FAILED_TO_ALLOCATE_MEMORY;
	}

	d_vector->start = new_vector_start;
	d_vector->current = (char *)d_vector->start + current_to_start;
	d_vector->end = (char *)d_vector->start + new_vector_capacity;

	return function_status;
}

d_vector_t *DVectorCreate(size_t size_of_element, size_t capacity)
{
	size_t vector_memory_bytes = capacity * size_of_element;

	d_vector_t *d_vector = malloc(sizeof(d_vector_t));
	if (NULL != d_vector)
	{
		return NULL;
	}

	d_vector->start = malloc(vector_memory_bytes);
	if (NULL == d_vector->start)
	{
		free(d_vector);
		d_vector = NULL;
	}

	d_vector->current = d_vector->start;
	d_vector->end = (char *)d_vector->start + vector_memory_bytes;
	d_vector->size_of_element = size_of_element;

	return d_vector;
}

void DVectorDestroy(d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	free(d_vector->start);
	d_vector->start = NULL;
	d_vector->current = NULL;
	d_vector->end = NULL;

	free(d_vector);
}

size_t DVectorSize(const d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	return ((size_t)d_vector->current - (size_t)d_vector->start) / 
												d_vector->size_of_element;
}

int DVectorIsEmpty(const d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	return (d_vector->current == d_vector->start);
}

static int IsDVectorFull(d_vector_t *d_vector)
{
	return (DVectorCapacity(d_vector) == DVectorSize(d_vector));
}

int DVectorPushBack(d_vector_t *d_vector, const void *data)
{
	int function_status = SUCCESS;

	assert(NULL != d_vector && NULL != data);

	if (IsDVectorFull(d_vector))
	{
		function_status = DVectorResize(d_vector, 
										DVectorCapacity(d_vector) *
										GROWTH_FACTOR);
		if (SUCCESS != function_status)
		{
			return function_status;
		}
	}

	memcpy(d_vector->current, data, d_vector->size_of_element);
	d_vector->current = (char *)d_vector->current + d_vector->size_of_element;

	return SUCCESS;
}

static int IsNumberOfElementInDVectorSmall(d_vector_t *d_vector)
{
	return (DVectorCapacity(d_vector) >= DVectorSize(d_vector) * SHRINK_FACTOR);
}

static int IsDVectorCapacityLargerThanShrinkFactor(d_vector_t *d_vector)
{
	return (DVectorCapacity(d_vector) >= SHRINK_FACTOR);
}

static int IsDVectorNeedShrink(d_vector_t *d_vector)
{
	return (IsNumberOfElementInDVectorSmall(d_vector) 
			&&
		   (IsDVectorCapacityLargerThanShrinkFactor(d_vector)));
}

int DVectorPopBack(d_vector_t *d_vector)
{
	int function_status = SUCCESS;

	assert(NULL != d_vector);
	assert(!DVectorIsEmpty(d_vector));

	d_vector->current = (char *)d_vector->current - d_vector->size_of_element;

	if (IsDVectorNeedShrink(d_vector))
	{
		function_status = DVectorResize(d_vector, DVectorCapacity(d_vector) / 													  SHRINK_FACTOR);
		if (function_status != SUCCESS)
		{
			return function_status;
		}
	}

	return SUCCESS;
}

void *DVectorGetItemAddress(const d_vector_t *d_vector, size_t index)
{
	assert(NULL != d_vector);
	assert(index < DVectorSize(d_vector));

	return (char *)d_vector->start + index * d_vector->size_of_element;
}

size_t DVectorCapacity(const d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	return ((size_t)d_vector->end - (size_t)d_vector->start) / 
											d_vector->size_of_element;
}

int DVectorReserve(d_vector_t *d_vector, size_t number_of_elements)
{
	assert(NULL != d_vector);
	assert(number_of_elements > DVectorSize(d_vector));

	return DVectorResize(d_vector, number_of_elements);
}

