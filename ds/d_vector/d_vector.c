#include "d_vector.h" /* d_vector_t */
#include <stdlib.h> /* malloc free */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */

#define GROWTH_FACTOR (2)
#define SHRINK_FACTOR (4)

enum function_status 
{
	OK = 0,
	FAILED_TO_ALLOCATE_MEMORY = -1,
	NUMBER_OF_ELEMENTS_SMALLER_THAN_CURRENT_SIZE = 1,
	VECTOR_ISEMPTY = 2
};

struct d_vector
{
	void *start;
	void *end;
	void *current; /*FIXME current what?*/
	size_t size_of_element;
};


static int DVectorResize(d_vector_t *d_vector, size_t number_of_elements)
{
	int function_status = OK;
	void *new_vector_start = NULL;
	ptrdiff_t current_to_start = (char *)d_vector -> current -
								 (char *)d_vector -> start;

	assert(NULL != d_vector);

	new_vector_start = realloc(d_vector -> start, number_of_elements * 
												  d_vector -> size_of_element);

	if (NULL == new_vector_start)
	{
		function_status = FAILED_TO_ALLOCATE_MEMORY;
	}
	else
	{
		d_vector -> start = new_vector_start;
		d_vector -> current = (char *)d_vector -> start + current_to_start;
		d_vector -> end = (char *)d_vector -> start + number_of_elements *
												    d_vector -> size_of_element;
	}

	return function_status;
}


d_vector_t *DVectorCreate(size_t size_of_element, size_t capacity)
{
	size_t vector_memory_bytes = capacity * size_of_element;
	d_vector_t *d_vector = malloc(sizeof(d_vector_t));

	if (NULL != d_vector)
	{
		d_vector -> start = malloc(vector_memory_bytes);

		if (NULL != d_vector -> start)
		{
			d_vector -> current = d_vector -> start;
			d_vector -> end = (char *)d_vector -> start + vector_memory_bytes;
			d_vector -> size_of_element = size_of_element;
		}
		else
		{
			free(d_vector);
			d_vector = NULL;
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


size_t DVectorSize(const d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	return ((size_t)d_vector -> current - (size_t)d_vector -> start) / 
													d_vector -> size_of_element;
}


int DVectorIsEmpty(const d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	return (d_vector -> current == d_vector -> start);
}


int DVectorPushBack(d_vector_t *d_vector, const void *data)
{
	size_t capacity_of_d_vector = DVectorCapacity(d_vector);
	int function_status = OK;

	assert(NULL != d_vector && NULL != data);

	if (capacity_of_d_vector == DVectorSize(d_vector))
	{
		function_status = DVectorResize(d_vector, 
										capacity_of_d_vector * GROWTH_FACTOR);
	}

	if (OK == function_status)
	{
		memcpy(d_vector -> current, data, d_vector -> size_of_element);
		d_vector -> current = (char *)d_vector -> current + 
													d_vector -> size_of_element;
	}

	return function_status;
}


int DVectorPopBack(d_vector_t *d_vector)
{
	size_t capacity_of_d_vector = DVectorCapacity(d_vector);
	int function_status = OK;

	assert(NULL != d_vector);

	if (DVectorIsEmpty(d_vector))
	{
		function_status = VECTOR_ISEMPTY;
	}
	else
	{
		d_vector -> current = (char *)d_vector -> current - 
													d_vector -> size_of_element;

		if (capacity_of_d_vector >= DVectorSize(d_vector) * SHRINK_FACTOR &&
										 capacity_of_d_vector >= SHRINK_FACTOR)
		{
			function_status = DVectorResize(d_vector, capacity_of_d_vector / 														  SHRINK_FACTOR);
		}
	}

	return function_status;
}


void *DVectorGetItemAddress(const d_vector_t *d_vector, size_t index)
{
	assert(NULL != d_vector);

	return (index < DVectorSize(d_vector)) ? 
		   (char *)d_vector -> start + index * d_vector -> size_of_element : 				NULL;
}


size_t DVectorCapacity(const d_vector_t *d_vector)
{
	assert(NULL != d_vector);

	return ((size_t)d_vector -> end - (size_t)d_vector -> start) / 
													d_vector -> size_of_element;
}


int DVectorReserve(d_vector_t *d_vector, size_t number_of_elements)
{
	int function_status = OK;

	assert(NULL != d_vector);

	if (number_of_elements < DVectorSize(d_vector))
	{
		function_status = NUMBER_OF_ELEMENTS_SMALLER_THAN_CURRENT_SIZE;
	}
	else
	{
		function_status = DVectorResize(d_vector, number_of_elements);
	}

	return function_status;
}

