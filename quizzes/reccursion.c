#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "linked_list.h" /* slist_node_t */

#define OK (printf("\033[1;32mOK.\033[0m\n"))
#define FAIL(name_of_test) (printf("\033[1;31mFAIL:\033[0m\n%s\nline: %d",name_of_test, __LINE__))
#define NEW_LINE (printf("\n\n"))

size_t IterativeFibonnachi(size_t n)
{
    size_t i = 0;
    size_t fib[2] = {0,1};

    for (i = 0; i < n; ++i)
    {
        size_t save_fib = fib[0];
        fib[0] = fib[1];
        fib[1] += save_fib;
    }
    
    return fib[0];
}

size_t ReccursionFibonnachi(size_t n)
{
    if (n < 2)
    {
        return n;
    }
    
    return ReccursionFibonnachi(n - 2) + ReccursionFibonnachi(n - 1);
}

size_t RecStrLen(const char *str)
{
    return ('\0' == *str)? 0 : (1 + RecStrLen(str + 1));
}

int RecStrCmp(const char *str1, const char *str2)
{
    return ('\0' != *str1 && *str1 == *str2)? 
           RecStrCmp(str1 + 1, str2 + 1) : (*str1 - *str2);
}

char *RecStrCpy(char *dest, const char *src)
{
    char *start_of_dest = dest;

    if ('\0' != *src)
    {
        *dest = *src;
        RecStrCpy(dest + 1, src + 1);
    }

    *dest = *src;
    return start_of_dest;
}

/*
char *RecStrCat(char *dest, const char *src)
{
    char *start_of_dest = dest;
    char *end_of_dest = dest + strlen(dest);

    if ('\0' != *src)
    {
        *dest = *src;
        RecStrCat(dest + 1, src + 1);
    }

    *dest = *src;
    return start_of_dest;
}
*/

char *RecStrStr(const char *haystack, const char *needle)
{
    if (strncmp(haystack, needle, strlen(needle)) == 0)
    {
        return (char *)haystack;
    }
    
    return RecStrStr(haystack + 1, needle);
}

slist_node_t *RecSlistFlip(slist_node_t *head)
{
    slist_node_t *current_node = NULL;
    
    assert(NULL != head);

    if (NULL == head->next_node)
    {
        return head;
    }
    
    current_node = RecSlistFlip(head->next_node);
    
    head->next_node->next_node = head;
    head->next_node = NULL;
    
    return current_node;
}

int AddNumToData(void *data, void* num)
{
	*((int*)data) += *((int*)num);
	
	return 0;
}

static void TestFlow2()
{
	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int test_param_for_action = 10;
	int expected_results_after_action[] = {11, 12, 13, 14, 15};
	int *expected_results_after_flip[5]; 
	size_t i = 0; 
	
	
	slist_node_t *test_head_node_for_action = NULL;
	slist_node_t *new_head_after_flip = NULL;
	slist_node_t *new_head_after_flip_cpy = NULL;
	
	slist_node_t *slist_node1 = SlistCreateNode(&a, NULL); /* head */
	slist_node_t *slist_node2 = SlistCreateNode(&b, NULL);
	slist_node_t *slist_node4 = SlistCreateNode(&d, NULL);
	slist_node_t *slist_node3 = SlistCreateNode(&c, NULL);
	slist_node_t *slist_node5 = SlistCreateNode(&e, NULL);
	
	printf("This is a test for Flow2:\n\n");
	
	SlistInsertAfterNode(slist_node1, slist_node2);
	SlistInsertAfterNode(slist_node2, slist_node3);
	SlistInsertAfterNode(slist_node3, slist_node4);
	SlistInsertAfterNode(slist_node4, slist_node5);
	
	expected_results_after_flip[0] = &e;
	expected_results_after_flip[1] = &d;
	expected_results_after_flip[2] = &c;
	expected_results_after_flip[3] = &b;
	expected_results_after_flip[4] = &a;
	
	
	/* #1 Try ForEach */
	if (0 == SlistForEach(slist_node1, AddNumToData, &test_param_for_action))
	{
		test_head_node_for_action = slist_node1;
		
		while (NULL != test_head_node_for_action)
		{
			if (expected_results_after_action[i] !=
					 *((int*)test_head_node_for_action->data))
			{
				FAIL("Check if data of each of the nodes match expected results");
				break;
			}
			else
			{
				OK;
			}
			
			++i;
			test_head_node_for_action = test_head_node_for_action->next_node;
		}
	}
	else
	{
		FAIL("Try to action func");
	}
	
	
	/* #2 Count same amount of nodes to flipped list */
	new_head_after_flip = RecSlistFlip(slist_node1);
	
	if (5 == (SlistCount(new_head_after_flip)))
	{
		OK;
	}
	else
	{
		FAIL("Count 5 nodes");
	}
	
	
	new_head_after_flip_cpy = new_head_after_flip;
	
	/* #3 Check the order of the nodes in the flipped list by matching their 
		data to the expected results */
	i = 0;
	while (NULL != new_head_after_flip)
	{
		if (expected_results_after_flip[i] == new_head_after_flip->data)
		{
			OK;
		}
		else
		{
			FAIL("Check if the flipped list match the expected result");
		}
		
		++i;
		
		new_head_after_flip = new_head_after_flip->next_node;
	}
	
	
	SlistFreeAll(new_head_after_flip_cpy);
	
	NEW_LINE;
}

void Test_My_strcpy()
{
    const char *str1 = "strcpy"; const char *str2 = "";
    char dest[7] = "aaaaaa";
    
    assert(RecStrCpy(dest, str1) == strcpy(dest, str1));
    assert(strcmp(RecStrCpy(dest, str1), strcpy(dest, str1)) == 0);
    assert(strcmp(RecStrCpy(dest, str2), strcpy(dest, str2)) == 0);
}


int main()
{
    printf("%lu\n", IterativeFibonnachi(0));
    printf("%lu\n", IterativeFibonnachi(5));
    printf("%lu\n", IterativeFibonnachi(52));
    
    printf("%lu\n", ReccursionFibonnachi(0));
    printf("%lu\n", ReccursionFibonnachi(5));
    
    Test_My_strcpy();
    TestFlow2();
    return 0;
}
