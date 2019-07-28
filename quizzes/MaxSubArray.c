#include <stdio.h>

typedef struct maxsubarray
{
	size_t index1;
	size_t index2;
	int sum;
} max_subarray_t;


max_subarray_t MaxSubArray(const int *array, size_t length)
{
	max_subarray_t result = {0,0,0};
	size_t i = 0;
	int tmp_sum = 0;
	size_t tmp_index1 = 0;

	for (i = 0; i < length; ++i)
	{
		if (tmp_sum < 0)
		{
			tmp_index1 = i;
			tmp_sum = 0;
		}
		
		if (array[i] + tmp_sum >= result.sum)
		{
			result.sum = tmp_sum + array[i];
			result.index2 = i;
			tmp_sum = result.sum;
			result.index1 = tmp_index1;
		}
		else
		{
			tmp_sum += array[i];
		}
	}

	return result;
}


void MaxSubArrayTest()
{
	int numoftests = 6;
	int array[6][6] =  {{1, 1, 1, 1, 1,  1}, {-1, -1, -1, 1, -1, -1}, 
						{0, 1, -2, 1, 2, 3}, { 1,  5, -2, 5, -10, 5},
						{1, -3, 1, 1, 1, 1}, {-1, -1,  5, 1, -10, 2}};

	max_subarray_t expected_result[6] = {{0, 5, 6}, {3, 3, 1}, {3, 5, 6},
										 {0, 3, 9}, {2, 5, 4}, {2, 3, 6}};
	int i = 0;

	for (i = 0; i < numoftests; ++i)
	{
		max_subarray_t result = MaxSubArray(array[i],6);
		printf("Max sum is: %d expected result is %d\n", result.sum, expected_result[i].sum);
		printf("First index is: %lu expected result is %lu\n", result.index1, expected_result[i].index1);
		printf("Second index is: %lu expected result is %lu\n", result.index2, expected_result[i].index2);
	}
}

int main()
{
	MaxSubArrayTest();
	return 0;
}
