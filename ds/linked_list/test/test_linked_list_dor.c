#include <stdio.h> /* printf */
#include <string.h> /* strcmp */

#include "linked_list.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

static void TestCreateFree()
{
	char *data = "dor tokea node";
	slist_node_t *node = SlistCreateNode(data, NULL);
	size_t error_count = 0;

	if (node->next_node != NULL)
	{
		FAIL("first created node doesn't point to NULL");
		++error_count;
	}

	if (data != (char *)node->data)
	{
		FAIL("data in node is not the data inserted");
		++error_count;
	}

	SlistFreeAll(node);

	if (0 == error_count)
	{
		PASS("create and free works");
	}
}


static void TestInsertAndInsertAfter()
{
	char *data = "dor tokea node";
	int dor = 77;
	size_t kiki = 90;
	int arr[5] = {0,1,2,3,4};

	slist_node_t *node = SlistCreateNode(data, NULL);
	slist_node_t *node2 = SlistCreateNode(&dor, node);
	slist_node_t *node3 = SlistCreateNode(&kiki, NULL);
	slist_node_t *node4 = SlistCreateNode(arr, NULL);
	size_t error_count = 0;

	SlistInsertNode(node, node3);

	if (node3->next_node != NULL)
	{
		FAIL("the added node (node3) doesn't point on the correct node");
		++error_count;
	}

	if (data != (char *)node3->data)
	{
		FAIL("the data on node3 is not correct");
		++error_count;
	}

	if (node2->next_node != node)
	{
		FAIL("the node (node2) doesn't point on the correct node");
		++error_count;
	}

	if (dor != *(int *)node2->data)
	{
		FAIL("the data on node2 is not correct");
		++error_count;
	}

	if (node->next_node != node3)
	{
		FAIL("the node (node) doesn't point on the correct node");
		++error_count;
	}

	if (kiki != *(size_t *)node->data)
	{
		FAIL("the data on node is not correct");
		++error_count;
	}

	SlistInsertAfterNode(node3, node4);

	if (node3->next_node != node4)
	{
		FAIL("node3 doesn't point on the added node");
		++error_count;
	}

	if (node4->next_node != NULL)
	{
		FAIL("the added node (node4) doesn't point on the right node");
		++error_count;
	}

	SlistFreeAll(node2);

	if (0 == error_count)
	{
		PASS("Insert and InsertAfter works");
	}
}

static void TestRemoveAndRemoveAfter()
{
	char *data = "dor tokea node";
	int dor = 77;
	size_t kiki = 90;
	int arr[5] = {0,1,2,3,4};

	slist_node_t *node = SlistCreateNode(data, NULL);
	slist_node_t *node2 = SlistCreateNode(&dor, node);
	slist_node_t *node3 = SlistCreateNode(&kiki, node2);
	slist_node_t *node4 = SlistCreateNode(arr, node3);
	size_t error_count = 0;

	if (SlistCount(node4) != 4)
	{
		FAIL("count didnt work");
		++error_count;
	}

	SlistRemove(node3);

	if (SlistCount(node4) != 3)
	{
		FAIL("count didnt work");
		++error_count;
	}

	SlistFreeAll(node4);

	if (0 == error_count)
	{
		PASS("count works");
	}
}


static void TestCount()
{
	char *data = "dor tokea node";
	int dor = 77;
	size_t kiki = 90;
	int arr[5] = {0,1,2,3,4};

	slist_node_t *node = SlistCreateNode(data, NULL);
	slist_node_t *node2 = SlistCreateNode(&dor, node);
	slist_node_t *node3 = SlistCreateNode(&kiki, node2);
	slist_node_t *node4 = SlistCreateNode(arr, node3);
	size_t error_count = 0;

	if (SlistCount(node4) != 4)
	{
		FAIL("count didnt work");
		++error_count;
	}

	SlistFreeAll(node4);

	if (0 == error_count)
	{
		PASS("count works");
	}
}

static int divide(void *arr, void *arr2)
{
	if (0 != *(int *)arr2)
	{
		return 1
	}
	else
	{
		*(int *)arr /= *(int *)arr2;
	}

	return 0;
}

static void TestForEach()
{

}

int main()
{
	TestCreateFree();
	TestInsertAndInsertAfter();
	TestRemoveAndRemoveAfter();
	TestCount();
	return 0;
}
