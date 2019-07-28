#include <stdio.h>

typedef struct maxsubarray
{
	int index1;
	int index2;
	int sum;
} max_subarray_t;


max_subarray_t MaxSubArray(const int *array, size_t length)
{
	max_subarray_t result = {0,0,0};
	size_t i = 0;
	int tmp_sum = array[0];

	for (i = 1; i < length; ++i)
	{
		if (tmp_sum < 0)
		{
			result.index1 = i;
			tmp_sum = 0;
		}
		
		if (array[i] + tmp_sum > result.sum)
		{
			result.sum = tmp_sum + array[i];
			result.index2 = i;
			tmp_sum = result.sum;
		}
		else
		{
			tmp_sum = result.sum + array[i];
		}
	}

	return result;
}


int main()
{
	int array[] = {-2, 1, -3, 4, -1, 2, 3, -5, 6};
	size_t length = sizeof(array)/sizeof(*array);
	
	max_subarray_t result = MaxSubArray(array,length);
	
	printf("Max sum is: %d\n", result.sum);
	printf("First index is: %d\n", result.index1);
	printf("Second index is: %d\n", result.index2);

	return 0;
}
