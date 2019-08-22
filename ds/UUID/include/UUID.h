#ifndef ILRD_UUID_H
#define ILRD_UUID_H
#include <sys/types.h> /* size_t, time_t, pid_t */

/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Thursday August 22 2019 04:09:20 PM */
/* This file contains functions for UUID data structure operations.*/
/* Can be used for UUID based algorithms */
/*############################################################################*/

typedef struct uuid
{
	size_t serial_id;
	time_t time;
	pid_t pid;
} uuid_t;

/*############################################################################
  # Description: 
	Creates a unique and different id.
  # Parameters:
	None
  # Return Value:
	Struct with unique id
############################################################################*/
uuid_t UUIDCreate(void);

/*############################################################################
  # Description:
	Checks if id's are the same
  # Parameters:
	uid1 - first uid to compare
	uid2 - second uid to compare
  # Return Value:
	1 if ids are the same, 0 otherwise

############################################################################*/
int UUIDIsSame(uuid_t uid1, uuid_t uid2);

#endif /* ILRD_UUID_H */
