#include <stdio.h>
#include <limits.h>
#include <assert.h>
#include "dhcp.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)
#define NEW_LINE printf("\n")

static void PrintIP(const unsigned char ip_address[BYTES_IN_IP])
{
	size_t j = 0;
	
	for (; j < BYTES_IN_IP; ++j)
	{
		printf("%d.", (int)ip_address[j]);
	}
	printf("\n");
}

static int IsMatchArrays(unsigned char results[][BYTES_IN_IP], 
						 unsigned char expected[][BYTES_IN_IP],
						 size_t num_of_rows)
{
	size_t i = 0;
	
	for (; i < num_of_rows; ++i)
	{
		size_t j = 0;
		
		for (; j < BYTES_IN_IP; ++j)
		{
			if (results[i][j] != expected[i][j])
			{
				return 0;
			}
		}
	}
	
	return 1;
}

static void TestFlow1()
{
	unsigned char net_id[BYTES_IN_IP] = {156, 77, 0, 0};
	size_t num_bits_net_id = 16;
	unsigned char ip_addresses[][BYTES_IN_IP] = {
											{156, 77, 0, 0},/*reserved*/
											{156, 77, 255, 254},/*reserved*/
											{156, 77, 255, 255},/*reserved*/
											{156, 77, 230, 0},
											{156, 77, 1, 255},
											{156, 77, 66, 254},
											{156, 77, 230, 0}/*already allocated*/
										};
	unsigned char expected[][BYTES_IN_IP] = {
										{156, 77, 0, 1},
										{156, 77, 0, 2},
										{156, 77, 0, 3},
										{156, 77, 230, 0},
										{156, 77, 1, 255},
										{156, 77, 66, 254},
										{156, 77, 230, 1}
									};
									
	unsigned char results[100][BYTES_IN_IP];
	
	unsigned char illegal_ip[BYTES_IN_IP] = {122, 5, 7, 16};
	
	size_t num_of_rows = sizeof(ip_addresses) / sizeof(*ip_addresses);
	
	size_t num_possible_ips = (1 << (BYTES_IN_IP * CHAR_BIT - num_bits_net_id)) 
	                           - 3;
	
	size_t i = 0;
	
	dhcp_t *dhcp = DHCPCreate(net_id, num_bits_net_id);
	assert(NULL != dhcp);
	
	assert(num_possible_ips == DHCPCountFree(dhcp));
	
	for (i = 0; i < num_of_rows; ++i)
	{
		assert(SUCCESS == DHCPLease(dhcp, ip_addresses[i], results[i]));
		assert(--num_possible_ips == DHCPCountFree(dhcp));
	}
	
	if (!IsMatchArrays(results, expected, num_of_rows))
	{
		FAIL("Allocated IPs don't match the expected results:");
		NEW_LINE;
		
		puts("net id:");
		PrintIP(net_id);
		NEW_LINE;
		
		for (i = 0; i < num_of_rows; ++i)
		{
			puts("requested ip:");
			PrintIP(ip_addresses[i]);
			puts("result ip:");
			PrintIP(results[i]);
			puts("expected ip:");
			PrintIP(expected[i]);
			
			NEW_LINE;
		}
		
		return;
	}
	
	assert(INVALID_REQUEST == DHCPRelease(dhcp, illegal_ip));
	
	for (i = 0; i < num_of_rows; ++i)
	{
		assert(SUCCESS == DHCPRelease(dhcp, results[i]));
		assert(++num_possible_ips == DHCPCountFree(dhcp));
	}
	
	assert(INVALID_REQUEST == DHCPRelease(dhcp, results[0]));
	
	assert(INVALID_REQUEST == DHCPLease(dhcp, illegal_ip, results[i]));
	
	DHCPDestroy(dhcp);
	dhcp = NULL;
	
	PASS("TESTFLOW1");
	NEW_LINE;
}

static void TestFlow2()
{
	unsigned char net_id[BYTES_IN_IP] = {210, 7, 0, 0};
	size_t num_bits_net_id = 16;
	
	size_t num_possible_ips = (1 << (BYTES_IN_IP * CHAR_BIT - num_bits_net_id)) 
	                           - 3;
	
	unsigned char ip_address[BYTES_IN_IP] = {210,7,0,0};
	unsigned char result[BYTES_IN_IP];
	
	dhcp_t *dhcp = DHCPCreate(net_id, num_bits_net_id);
	assert(NULL != dhcp);
	
	while (0 < num_possible_ips)
	{
		assert(num_possible_ips == DHCPCountFree(dhcp));
		
		assert(SUCCESS == DHCPLease(dhcp, ip_address, result));
		
		/*printf("Free IPs left: %lu\n", num_possible_ips);
		printf("result: ");
		PrintIP(result);
		NEW_LINE;*/
		
		--num_possible_ips;
	}
	
	assert(INVALID_REQUEST == DHCPLease(dhcp, ip_address, result));
	
	PASS("TESTFLOW2");
	NEW_LINE;
}


int main()
{
    TestFlow1();
    TestFlow2();

    return 0;
}
