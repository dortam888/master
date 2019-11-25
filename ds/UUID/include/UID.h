/*############################################################################*/
/* Owner: Dor Tambour */
/* Reviewer: Dor Tambour */
/* Create: Thursday August 22 2019 04:09:20 PM */
/* This file contains functions for UID data structure operations.*/
/* Can be used for UID based algorithms */
/*############################################################################*/

#ifndef UID_H
#define UID_H
#include <sys/types.h> /* size_t, time_t, pid_t */

typedef struct uid
{
	size_t counter;
	time_t time;
	pid_t pid;
} uid_t;

extern const uid_t BAD_UID;

/*############################################################################
  # Description: 
	Creates a unique and different id.
  # Parameters:
	None
  # Return Value:
	Struct with unique id
############################################################################*/
uid_t UIDCreate(void);

/*############################################################################
  # Description:
	Checks if id's are the same
  # Parameters:
	uid1 - first uid to compare
	uid2 - second uid to compare
  # Return Value:
	1 if ids are the same, 0 otherwise

############################################################################*/
int UIDIsSame(uid_t uid1, uid_t uid2);

#endif /* UID_H */
