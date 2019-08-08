#ifndef ILRD_VECTOR_H
#define ILRD_VECTOR_H
#include <stddef.h> /* size_t */

typedef struct d_vector d_vector_t;

/*#############################################################################
  # Description: 
	get size of element and intial capacity and create dynamic vector
  # Parameters:
	size_of_element: size of single data element in bytes
  # Return Value:
	in case of success returns a pointer to the dynamic vector itself,
	in case of failure returns NULL
##############################################################################*/
d_vector_t *DVectorCreate(size_t size_of_element, size_t capacity);

/*#############################################################################
  # Description: 
	get dynamic vector and destroy it
  # Parameters:
	d_vector - the pointer to the dynamic vector 
  # Return Value:
	None
##############################################################################*/
void DVectorDestroy(d_vector_t *d_vector);


/*#############################################################################
  # Description: 
	return how many elements are currently inside the dynamic vector
  # Parameters:
	d_vector - the pointer to the dynamic vector 
  # Return Value:
	number of elements in dynamic vector
##############################################################################*/
size_t DVectorSize(const vector_t *d_vector);


/*#############################################################################
  # Description: 
	check if dynamic vector is empty
  # Parameters:
	d_vector - the pointer to the dynamic vector 
  # Return Value:
	1 if there are no elements in the dynamic vector 0 otherwise
##############################################################################*/
int DVectorIsEmpty(const vector_t *d_vector);

/*#############################################################################
  # Description: 
	add data element to the dynamic vector
  # Parameters:
	d_vector - the pointer to the dynamic vector from where we push
	data - data that should be stored in the dynamic vector
  # Return Value:
	returns 0 in case of success, and other integer value in case of failure
##############################################################################*/
int DVectorPushBack(vector_t *d_vector, const void *data);

/*#############################################################################
  # Description: 
	remove data element to the dynamic vector
  # Parameters:
	d_vector - the pointer to the dynamic vector where we pop
  # Return Value:
	returns 0 in case of success, and other integer value in case of failure
##############################################################################*/
int DVectorPopBack(vector_t *d_vector);

/*#############################################################################
  # Description: 
	return the start address of requested element in the dynamic vector 
	refered by index
  # Parameters:
	d_vector - pointer to the dynamic vector
  # Return Value:
	address of the requested element
##############################################################################*/
void *DVectorGetItemAddress(const vector_t *d_vector, size_t index);

/*#############################################################################
  # Description: 
	return max amount of elements of the dynamic vector 
  # Parameters:
	d_vector - pointer to the dynamic vector
  # Return Value:
	size of dynamic vector
##############################################################################*/
size_t DVectorCapacity(const vector_t *d_vector);

/*#############################################################################
  # Description: 
	set the size of dynamic vector to be able to hold requested 
	number of elements
  # Parameters:
	d_vector - pointer to the dynamic vector
	number_of_elements - size of vector after change
  # Return Value:
	returns 0 in case of success, 
	returns 1 if new size is smaller then curren capacity, 
	dynamic vector size is not changed
	returns -1 if allocation didn't succeded
##############################################################################*/
int DVectorReserve(vector_t *d_vector, size_t number_of_elements);

#endif /* ILRD_VECTOR_H */

