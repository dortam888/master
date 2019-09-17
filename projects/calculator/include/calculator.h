/*############################################################################*/
/* Owner: OL712                                                               */
/* Reviewer: OL712                                                            */
/*############################################################################*/

#ifndef ILRD_CALCULATOR_H
#define ILRD_CALCULATOR_H

typedef enum calc_errno
{
	CALC_SUCCESS,
	CALC_FAILED_ALOC,
	CALC_ERR_INVALID_CALCULATION,
	CALC_ERR_INVALID_EXPRESSION,
	CALC_OVERFLOW
}calc_errno_t;

/*
 * Calculates double based on <expression>.
 * @expression: non-empty null terminated string containing a mathematical 
 *				expression.
 *				a supported mathematical expression must be a valid expression
 *					and can include:
 *				double type numbers,
 *				the operators: '+', '-', '*', '/', '^',
 *				unary operators: '+', '-' before numbers or opening parentheses,
 *				parentheses: '(', ')',
 *				white space between anything except unary operators and their
 *					operand. white space includes everything that is counted as
 *					space in the isspace function in <ctype.h> library.
 * return : result of <expression> and updates calc_errno to CALC_SUCCESS.
 * errors : if error encountered,
 *			returns 0 and updates <errno> accordingly.
 *			if <expression> is not null terminated, behaviour is undefined.
 */
double Calculator(const char *expression, calc_errno_t *errno);

#endif /* ILRD_CALCULATOR_H */
