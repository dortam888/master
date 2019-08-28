#include <unistd.h> /*getpid*/
#include <time.h> /* time */

#include "UID.h"

const ilrd_uid_t BAD_UID = {0, -1, 0xDEAD91D};

ilrd_uid_t UIDCreate(void)
{
	ilrd_uid_t new_uid = {0, 0, 0};
	static size_t counter;
	
	new_uid.counter = counter++;
	new_uid.pid = getpid();
	new_uid.time = time(NULL);
	if (-1 == new_uid.time)
	{
		return BAD_UID;
	}
	
	return new_uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return((uid1.counter == uid2.counter) &&
		(uid1.time == uid2.time) &&
		(uid1.pid == uid2.pid));

}
