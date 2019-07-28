#include <stdio.h>
#include <stdlib.h> /*size_t*/

typedef enum function_status {Could_Not_Allocate_Memory = -1, OK = 0} function_status_t;


typedef struct data_members
{
	void *data;
	function_status_t(*add)(int, struct data_members *);
	void(*print)(struct data_members *);
	void(*free)(struct data_members *);
} data_members_t;


typedef struct DateOfBirth
{
	size_t day;
	size_t month;
	size_t year;
} dateofbirth_t;


typedef struct Contact
{
	char f_name[32];
	char l_name[32];
	size_t ID;
	dateofbirth_t dob;
} contact_t;


static FILE *OpenFile(const char *file_name, char *access);
void ContactList();


int main()
{
	ContactList();
	return 0;
}


void ContactList()
{
	contact_t members[10] = {{"Juan", "Travolt", 234522018, {6, 5, 1975}},
							 {"Jonas", "Nuebuer", 254300087, {23, 1, 1985}},
							 {"Larry", "Long",  198874322, {12, 12, 1986}},
							 {"Chris", "Corny", 364721555, {31, 7, 1990}},
							 {"Ian", "Black", 144666212, {4, 4, 1951}},
							 {"Jacko", "Sparring", 200134898, {17, 9, 1966}},
							 {"Bulid", "Ashtanga", 368745521, {30, 2, 1991}},
							 {"Vukmir", "Vukmir", 377946513, {15, 4, 1992}},
							 {"Kobe", "Tamarandi", 397461255, {8, 7, 2000}},
							 {"Peter", "Gabe", 134687964, {13, 6, 1975}}};

	contact_t reading_array[10];
	char *file_name = "Contacts_List";
	FILE *file = OpenFile(file_name, "w");
	fwrite(members, sizeof(contact_t) * 10, 1, file);
	fclose(file);

	file = OpenFile(file_name, "r");
	fread(reading_array, sizeof(contact_t) * 10, 1, file);
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


