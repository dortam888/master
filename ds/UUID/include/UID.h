/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Thursday August 22 2019 04:09:20 PM */
/* This file contains functions for UID data structure operations.*/
/* Can be used for UID based algorithms */
/*############################################################################*/

#ifndef ILRD_UID_H
#define ILRD_UID_H
#include <sys/types.h> /* size_t, time_t, pid_t */

typedef struct uid
{
	size_t counter;
	time_t time;
	pid_t pid;
} ilrd_uid_t;

extern const ilrd_uid_t BAD_UID;

/*############################################################################
  # Description: 
	Creates a unique and different id.
  # Parameters:
	None
  # Return Value:
	Struct with unique id
############################################################################*/
ilrd_uid_t UIDCreate(void);

/*############################################################################
  # Description:
	Checks if id's are the same
  # Parameters:
	uid1 - first uid to compare
	uid2 - second uid to compare
  # Return Value:
	1 if ids are the same, 0 otherwise

############################################################################*/
int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2);

#endif /* ILRD_UID_H */
