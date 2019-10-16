/*############################################################################*/
/* Owner: OL72 */
/* Reviewer: OL72 */
/* Create: Tuesday October 15 2019 05:32:14  */
/* This file contains functions for my_shell data structure operations.*/
/* Can be used for my_shell based algorithms */
/*############################################################################*/

#ifndef ILRD_MY_SHELL_H
#define ILRD_MY_SHELL_H

#include <stddef.h> /* size_t */

enum shell_status 
    {SUCCESS,
     EXIT,
     COULD_NOT_CREATE_CHILD_PROCESS,
     FAILED_TO_EXECUTE};

/*############################################################################
  # Description:

  # Parameters:

  # Return Value:

############################################################################*/
enum shell_status Shell();

#endif /* ILRD_MY_SHELL_H */
