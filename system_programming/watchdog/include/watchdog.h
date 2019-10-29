/****************************************
 * Title  : Watchdog				    *
 * Author : Dandan Ogni                 *
 * Group  : OL712                       *
 * Date   : 28/10/2019               	*
 ****************************************/

#ifndef __OL712_WATCHDOG_H
#define __OL712_WATCHDOG_H

#include <time.h> /*time_t*/

/*
 * set watchdog to resuscitate the calling process if needed
 * @argv: main's argv
 * @interval: interval in secs for sending signals between user process and
 *			  watchdog. must be at least 1.
 * Return: 0 if success, non-zero if failed
 * Errors: if failed, return non-zero.
 *         if argv[0] is not the process's executable, behaviour is undefined.
 *         if you call WDMakeMeImmortal twice without WDDoNotResuscitate,
 *		   behaviour is undefined.
 *         if you call for MMI with different intervals in the same code,
 *         behaviour is undefined.
 *         if calling setenv / unsetenv / getenv while watchdog is active,
 *         behaviour is undefined.
 * Note: while watchdog is active, don't use SIGUSR1, SIGUSR2.
 */
int WDMakeMeImmortal(char *argv[], time_t interval);


/*
 * unset watchdog
 * Return: none
 * Errors: if you call WDDoNotResuscitate without calling WDMakeMeImmortal,
 *		   behaviour is undefined.
 * Note: you must call WDDoNotResuscitate after a call to WDMakeMeImmortal,
 *		 even if the process was resuscitated.
 */
void WDDoNotResuscitate(void);


#endif /* __OL712_WATCHDOG_H */
