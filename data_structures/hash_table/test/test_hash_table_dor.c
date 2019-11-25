#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hash_table.h"

#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d\n", str, __LINE__)

#define HASH_SIZE 178
#define NUMBER_OF_WORDS_IN_DICTIONARY 99171
#define MAX_WORD_SIZE 40
#define DUMMY_INVALID_ADDRESS ((void *)0xDEADC0DE12DEAD)
#define UNUSED(x) ((void)x)

static size_t histogram[HASH_SIZE] = {0};

static size_t HashFunction(void *str)
{
    unsigned char *word = (unsigned char *)str;
    size_t hash_key = 0;
    
    while ('\0' != *word)
    {
        hash_key += *word;
        ++word;
    }
    
    ++histogram[hash_key % HASH_SIZE]; /* For Statistics */
    
    return hash_key % HASH_SIZE;
}

static int HashCmp(const void *key1, const void *key2)
{
    return strcmp((const char *)key1, (const char *)key2);
}

static int PrintAll(void *table_data, void *param)
{
    printf("%s\n", (char *)table_data);
    
    UNUSED(param);

    return 0;
}

static void Swap(char *str1, char *str2)
{
    char tmp = *str1;
    *str1 = *str2;
    *str2 = tmp;
}

static void FindPermutations(hash_table_t *hash_table, char *string, 
                             size_t start_index, size_t end_index)
{
    size_t i = 0;
    
    if (start_index == end_index)
    {
        if ((char *)HashTableFind(hash_table, string) == string)
        {
            printf("%s\n", string);
            HashTableRemove(hash_table, string);
        }
    }
    
    for (i = start_index; i < end_index; ++i)
    {
        Swap((string + start_index), (string + i));
        FindPermutations(hash_table, string, start_index + 1, end_index);
        Swap((string + start_index), (string + i));
    }
}

static double Variance()
{
    double mean = 0;
    double prob_mass = 1.0 / HASH_SIZE;
    double var = 0;
    size_t i = 0;

    for (i = 0; i < HASH_SIZE; ++i)
    {
        mean += histogram[i];
    }
    
    mean = mean / HASH_SIZE;
    
    for (i = 0; i < HASH_SIZE; ++i)
    {
        printf("bucket[%lu] = %lu\n", i, histogram[i]);
        var += prob_mass * (histogram[i] - mean) * (histogram[i] - mean);
    }
    
    return var;
}

static void TestDictionary()
{
    char **dict = NULL;
    size_t i = 0;
    size_t buckets = HASH_SIZE;
    char *data_to_find = "antiquate";
    char *data_to_find2 = "environmentalism's";
    char *data_to_find3 = "Ångström";
    char *data_not_to_find = "blablabla";
    size_t error_counter = 0;
    
    hash_table_t *new_hash = NULL;
    FILE *dictionary = fopen("words.txt", "r");
    if (NULL == dictionary)
    {
        printf("could not open file\n");
        return;
    }
    
    dict = (char **)malloc(NUMBER_OF_WORDS_IN_DICTIONARY * MAX_WORD_SIZE);
    
    for (i = 0; i < NUMBER_OF_WORDS_IN_DICTIONARY; ++i)
    {
        dict[i] = (char *)malloc(MAX_WORD_SIZE);
    }

    new_hash = HashTableCreate(buckets, HashFunction, HashCmp);
    if (NULL == new_hash)
    {
        printf("couldn't create hash_table\n");
        return;
    }

    for (i = 0; 0 == feof(dictionary); ++i)
    {
        fscanf(dictionary, "%s", dict[i]);
    }
    
    fclose(dictionary);

    if (!HashTableIsEmpty(new_hash))
    {
        ++error_counter;
        FAIL("should be empty");
    }
    
    for (i = 0; i < NUMBER_OF_WORDS_IN_DICTIONARY; ++i)
    {
        HashTableInsert(new_hash, dict[i]);
    }
    
    printf("variance of hash is: %f\n", Variance());
    
    if (HashTableSize(new_hash) != NUMBER_OF_WORDS_IN_DICTIONARY)
    {
        ++error_counter;
        FAIL("hash table size should be as the number of words in dictionary\n");
    }
    
    if (strcmp((char *)HashTableFind(new_hash, data_to_find), data_to_find))
    {
        ++error_counter;
        FAIL("Find didn't find");
    }
    
    if (strcmp((char *)HashTableFind(new_hash, data_to_find2), data_to_find2))
    {
        ++error_counter;
        FAIL("Find didn't find");
    }
    
    if (strcmp((char *)HashTableFind(new_hash, data_to_find3), data_to_find3))
    {
        ++error_counter;
        FAIL("Find didn't find");
    }
    
    if (HashTableFind(new_hash, data_not_to_find) != DUMMY_INVALID_ADDRESS)
    {
        ++error_counter;
        FAIL("Find should find");
    }
    
    /*HashTableForEach(new_hash, PrintAll, NULL);*/

    HashTableDestroy(new_hash);
    
    for (i = 0; i < NUMBER_OF_WORDS_IN_DICTIONARY; ++i)
    {
        free(dict[i]);
    }
    
    free(dict);

    if (0 == error_counter)
    {
        PASS("HashTable");
    }
}


int main()
{
    TestDictionary();
    return 0;
}
