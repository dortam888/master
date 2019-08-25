#include <stdio.h>

#include "UID.h"


#define PASS(str) printf("\033[0;32m%s\033[0m\n", str)
#define FAIL(str) printf("\033[0;31m%s\033[0m in line %d/n", str, __LINE__)

static void TestUid()
{
	ilrd_uid_t uid1 = UIDCreate();
	ilrd_uid_t uid2 = UIDCreate();
	
	if (UIDIsSame(uid1, uid2))
	{
		FAIL("shouldn't be the same");
	}
	
	if (!UIDIsSame(uid1, uid1))
	{
		FAIL("should be the same");
	}
}

int main()
{
	TestUid();
    return 0;
}
