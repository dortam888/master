#include <stdio.h>
#include <stdlib.h> /*size_t*/
#include <string.h> /* strcat, strcpy */
#include <assert.h> /* assert */


#define BYTES 8
#define NUM_OF_ELEMENTS 3
#define int2char(num) num + 48
#define LongestNameSize 50
#define NUM_OF_CONTACTS 10
#define Sizeof(var) (char *)(&var + 1) - (char *)&var



typedef enum function_status {Could_Not_Allocate_Memory = -1, OK = 0} function_status_t;


typedef struct data_members
{
	void *data;
	function_status_t(*add)(int, struct data_members *);
	void(*print)(struct data_members);
	void(*free)(struct data_members);
} data_members_t;


typedef struct DateOfBirth
{
	size_t day;
	size_t month;
	size_t year;
} dateofbirth_t;


typedef struct Contact
{
	char f_name[LongestNameSize];
	char l_name[LongestNameSize];
	size_t ID;
	dateofbirth_t dob;
} contact_t;


int StructOfFunctions();
data_members_t *InitializeDataMembersArray();
static FILE *OpenFile(const char *file_name, char *access);
void ContactList();


int main(int argc, char *argv[])
{
	StructOfFunctions();
	ContactList(argv[1]);
	(void)argc;
	return 0;
}


function_status_t IntAddFunc(int num_to_add, data_members_t *int_member)
{
	*(int *)int_member[0].data += num_to_add;
	return OK;
}


function_status_t FloatAddFunc(int num_to_add, data_members_t *float_member)
{
	*(float *)float_member[0].data += num_to_add;
	return OK;
}


size_t NumberOfDigitsInNum(int num)
{
	size_t number_of_digits = 0;
	
	while (0 != num)
	{
		++number_of_digits;
		num /= 10;
	}
	
	return number_of_digits;
}


function_status_t StringAddFunc(int num_to_add, data_members_t *string_member)
{

	size_t number_of_digits = NumberOfDigitsInNum(num_to_add);
	size_t needed_length = strlen(string_member[0].data) + number_of_digits + 1;
	
	string_member[0].data = realloc(string_member[0].data, needed_length);
	
	if (NULL == string_member[0].data)
	{
		return Could_Not_Allocate_Memory;
	}

	sprintf(string_member[0].data, "%s%d", (char *)string_member[0].data, num_to_add);
	
	return OK;
}


void IntPrintFunc(data_members_t int_member)
{
	printf("%d\n", *(int *)int_member.data);
}


void FloatPrintFunc(data_members_t float_member)
{
	printf("%f\n", *(float *)float_member.data);
}


void StringPrintFunc(data_members_t string_member)
{
	printf("%s\n", (char *)string_member.data);
}


void AlwaysTrue(data_members_t anything)
{
	(void)anything;
}


void FreeMemory(data_members_t string_member)
{
	assert(NULL != string_member.data);
	free(string_member.data);
	string_member.data = NULL;
}


data_members_t *InitializeDataMembersArray(data_members_t *array_of_members,
										   int *integer_number,
										   float *float_number,
										   char *string)
{
	function_status_t problem = Could_Not_Allocate_Memory;

	array_of_members[0].data = integer_number;
	array_of_members[0].add = IntAddFunc;
	array_of_members[0].print = IntPrintFunc;
	array_of_members[0].free = AlwaysTrue;

	array_of_members[1].data = float_number;
	array_of_members[1].add = FloatAddFunc;
	array_of_members[1].print = FloatPrintFunc;
	array_of_members[1].free = AlwaysTrue;

	array_of_members[2].data = malloc(strlen(string) + 1);
	array_of_members[2].add = StringAddFunc;
	array_of_members[2].print = StringPrintFunc;
	array_of_members[2].free = FreeMemory;

	if (array_of_members[NUM_OF_ELEMENTS - 1].data == NULL)
	{
		array_of_members[NUM_OF_ELEMENTS - 1].data = &problem;
		
		return array_of_members;
	}

	strcpy(array_of_members[NUM_OF_ELEMENTS - 1].data, string);

	return array_of_members;
}


int StructOfFunctions()
{
	int integer_number = 45;
	float float_number = 84.4;
	char string[] = "somestring";
	int i = 0;
	int num_to_add = 10;
	data_members_t array_of_members[NUM_OF_ELEMENTS];
	InitializeDataMembersArray(array_of_members, &integer_number,
							   &float_number, string);

	for (i = 0; i < NUM_OF_ELEMENTS; ++i)
	{
		array_of_members[i].add(num_to_add, &array_of_members[i]);
		array_of_members[i].print(array_of_members[i]);
		array_of_members[i].free(array_of_members[i]);
	}

	return 0;

}


void ContactList(char *file_name)
{
	contact_t members[NUM_OF_CONTACTS] = 
							 {{"Juan", "Travolt", 234522018, {6, 5, 1975}},
							 {"Jonas", "Nuebuer", 254300087, {23, 1, 1985}},
							 {"Larry", "Long",  198874322, {12, 12, 1986}},
							 {"Chris", "Corny", 364721555, {31, 7, 1990}},
							 {"Ian", "Black", 144666212, {4, 4, 1951}},
							 {"Jacko", "Sparring", 200134898, {17, 9, 1966}},
							 {"Bulid", "Ashtanga", 368745521, {30, 2, 1991}},
							 {"Vukmir", "Vukmir", 377946513, {15, 4, 1992}},
							 {"Kobe", "Tamarandi", 397461255, {8, 7, 2000}},
							 {"Peter", "Gabe", 134687964, {13, 6, 1975}}};

	contact_t reading_array[NUM_OF_CONTACTS];
	FILE *file = OpenFile(file_name, "w");
	fwrite(members, sizeof(contact_t) * NUM_OF_CONTACTS, 1, file);
	fclose(file);

	file = OpenFile(file_name, "r");
	fread(reading_array, sizeof(contact_t) * NUM_OF_CONTACTS, 1, file);
	fclose(file);
}


static FILE *OpenFile(const char *file_name, char *access)
{
    FILE *file = fopen(file_name, access);

    if (file == NULL)
    { 
        printf("Could not open file %s", file_name);
		exit(0);
    }

    return file;
}


