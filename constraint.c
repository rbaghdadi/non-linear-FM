#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <isl/polynomial.h>
#include <isl/set.h>
#include <isl/non-linear-FM/constraint.h>
#include <isl/non-linear-FM/domain.h>

#include "debug.h"

/* A domain described using quasi-polynomial constraints.  */
constraint *constraint_copy(constraint *src)
{
	assert(src);

	constraint *dest = (constraint *)
					malloc(sizeof(constraint));

	dest->constraint = isl_pw_qpolynomial_copy(src->constraint);
	dest->eq = src->eq;

	constraint_free(src);

	return dest;
}

void constraint_free(constraint *cst)
{
	isl_pw_qpolynomial_free(cst->constraint);

	free(cst);
}
