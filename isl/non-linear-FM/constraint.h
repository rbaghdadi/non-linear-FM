#ifndef H_CONSTRAINTS_GPS
#define H_CONSTRAINTS_GPS

#include <assert.h>
#include <stdio.h>
#include <isl/polynomial.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct constraint
{
	isl_pw_qpolynomial *constraint;
	unsigned char eq;
	struct constraint *next;
} constraint;

void constraint_free(constraint *cst);

/* A domain described using quasi-polynomial constraints.  */
constraint *constraint_copy(constraint *src);

#if defined(__cplusplus)
}
#endif

#endif
