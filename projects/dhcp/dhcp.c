/*******************************************************************************
**** Author: Dor Tambour
**** Last Update: שני אוקטובר 14 2019 09:51:34 am
**** Reviewer:
**** Description: This file contains the implementations of functions
                                  for the data structure dhcp.
                                  Look at dhcp.h for more information about the 
                                  functions.
*******************************************************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <limits.h> /* CHAR_BIT */

#include "dhcp.h" 
#include "trie.h" 

#define TAKEN_IPS (3)

struct dhcp
{
    trie_t *trie;
    size_t num_bits_net_id;
    unsigned int net_id;
};

static const size_t BITS_IN_IP = CHAR_BIT * BYTES_IN_IP;

static unsigned int GetNetIDMask(size_t num_bits_net_id)
{
    return ~(~(0x0u) >> num_bits_net_id);
}

static unsigned int GetIPAddressAsInt(const unsigned char ip_address[])
{
    size_t i = 0LU;
    unsigned int result = 0u;

    assert(NULL != ip_address);
    assert(BYTES_IN_IP > 0);

    for (i = 0; i < BYTES_IN_IP - 1; ++i)
    {
        result |= ip_address[i];
        result <<= CHAR_BIT;
    }

    result |= ip_address[i];

    return result;
}

static void IntIPToCharIP(unsigned int ip_adress, 
                          unsigned char dest[])
{
    size_t i = 0LU;

    assert(NULL != dest);
    assert(BYTES_IN_IP > 0);

    for (i = BYTES_IN_IP; i > 0; --i)
    {
        unsigned char mask = 0x0u;
        dest[i - 1] = mask | ip_adress;
        ip_adress >>= CHAR_BIT;
    }
}

static int IsNetIDValid(const dhcp_t *dhcp, unsigned int ip_address)
{
    unsigned int net_id_mask = 0x0u;

    assert(NULL != dhcp);

    net_id_mask = GetNetIDMask(dhcp->num_bits_net_id);

    return ((ip_address & net_id_mask) == dhcp->net_id);
}

static unsigned int PutNetIDToIP(const dhcp_t *dhcp, 
                                 unsigned int ip_address)
{
    unsigned int net_id_mask = 0x0u;

    assert(NULL != dhcp);

    net_id_mask = GetNetIDMask(dhcp->num_bits_net_id);
    ip_address &= ~(net_id_mask); 
    ip_address |= dhcp->net_id;

    return ip_address;
}

static int ReserveEssentialIPs(dhcp_t *dhcp)
{
    unsigned int essential_ips[TAKEN_IPS] = {0x0u, ~0x0u, (~0x0u) - 0x1u};
    size_t i = 0LU;

    assert(NULL != dhcp);

    for (i = 0LU; i < TAKEN_IPS; ++i)
    {
        unsigned char current_ip[BYTES_IN_IP] = {0};
        essential_ips[i] = PutNetIDToIP(dhcp, essential_ips[i]); 

        IntIPToCharIP(essential_ips[i], current_ip);

        if (SUCCESS != DHCPLease(dhcp, current_ip, current_ip))
        {
            return FAIL;
        }
    }

    return SUCCESS;
}

static int InitDHCP(dhcp_t *dhcp, const unsigned char net_id[],
                    size_t num_bits_net_id)
{
    assert(NULL != dhcp);
    assert(NULL != net_id);

    dhcp->num_bits_net_id = num_bits_net_id;

    dhcp->trie = TrieCreate(BITS_IN_IP - num_bits_net_id);
    if (NULL == dhcp->trie)
    {
        return FAIL;
    }

    dhcp->net_id = GetIPAddressAsInt(net_id) & GetNetIDMask(num_bits_net_id);

    if (SUCCESS != ReserveEssentialIPs(dhcp))
    {
        TrieDestroy(dhcp->trie); dhcp->trie = NULL;
        return FAIL;
    }

    return SUCCESS;
}

dhcp_t *DHCPCreate(const unsigned char net_id[BYTES_IN_IP], 
                   size_t num_bits_net_id)
{
    dhcp_t *new_dhcp = NULL;

    assert(NULL != net_id);

    new_dhcp = (dhcp_t *)malloc(sizeof(*new_dhcp));
    if (NULL == new_dhcp)
    {
        return NULL;
    }

    if (InitDHCP(new_dhcp, net_id, num_bits_net_id) != SUCCESS)
    {
        free(new_dhcp); new_dhcp = NULL;
        return NULL;
    }

    return new_dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);

    TrieDestroy(dhcp->trie); dhcp->trie = NULL;
    free(dhcp); dhcp = NULL;
}

static insertion_status_t LeaseNextFreeIP(dhcp_t *dhcp, 
                                          unsigned int *ptr_current_ip)
{
    unsigned last_legal_ip = ((~0x0u) - 0x2u);
    insertion_status_t status = ALREADY_INSERTED;

    assert(NULL != dhcp);
    assert(NULL != ptr_current_ip);

    last_legal_ip = PutNetIDToIP(dhcp, last_legal_ip);

    do
    {
        if (last_legal_ip <= *ptr_current_ip)
        {
            /* set current ip to be the first legal ip address */
            *ptr_current_ip = PutNetIDToIP(dhcp, 0x1u);
        }
        else
        {
            /* try to allocate next ip address */
            ++*ptr_current_ip; 
        }

        status = TrieInsert(dhcp->trie, *ptr_current_ip);
    } while (ALREADY_INSERTED == status);

    return status;
}

dhcp_status_t DHCPLease(dhcp_t *dhcp, const unsigned char req_ip[],
                        unsigned char alloc_ip[])
{
    unsigned int req_ip_adress = 0x0u;
    insertion_status_t status = ALREADY_INSERTED;

    assert(NULL != dhcp);
    assert(NULL != req_ip);
    assert(NULL != alloc_ip);

    req_ip_adress = GetIPAddressAsInt(req_ip);

    if (!IsNetIDValid(dhcp, req_ip_adress) ||
        0 == DHCPCountFree(dhcp)) 
    {
        return INVALID_REQUEST;
    }

    status = TrieInsert(dhcp->trie, req_ip_adress);

    if (ALREADY_INSERTED == status)
    {
        status = LeaseNextFreeIP(dhcp, &req_ip_adress);
    }

    else if (INSERT_SUCCESS != status)
    {
        return FAIL;
    }

    IntIPToCharIP(req_ip_adress, alloc_ip);
    return SUCCESS;
}

dhcp_status_t DHCPRelease(dhcp_t *dhcp, const unsigned char ip_to_release[])
{
    unsigned int ip_to_release_adress = 0x0u;

    assert(NULL != dhcp);
    assert(NULL != ip_to_release);

    ip_to_release_adress = GetIPAddressAsInt(ip_to_release);

    if (!IsNetIDValid(dhcp, ip_to_release_adress) ||
        !TrieIsKeyFound(dhcp->trie, ip_to_release_adress))
    {
        return INVALID_REQUEST;
    }

    TrieRemove(dhcp->trie, ip_to_release_adress);
    return SUCCESS;
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
    size_t num_bits_host_id = 0LU;
    size_t num_of_possible_ips = 0LU;

    assert(NULL != dhcp);

    num_bits_host_id = BITS_IN_IP - dhcp->num_bits_net_id;
    num_of_possible_ips = (0x1u << num_bits_host_id);

    return num_of_possible_ips - TrieCountUsedPaths(dhcp->trie);
}

