#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <isl/polynomial.h>
#include <isl/set.h>
#include <isl/non-linear-FM/nfm_constraint.h>
#include <isl/non-linear-FM/nfm_domain.h>

#include "debug.h"

/* A domain described using quasi-polynomial constraints.  */
nfm_constraint *nfm_constraint_copy(nfm_constraint *src)
{
	assert(src);

	nfm_constraint *dest = (nfm_constraint *)
					malloc(sizeof(nfm_constraint));

	dest->constraint = isl_pw_qpolynomial_copy(src->constraint);
	dest->eq = src->eq;

	nfm_constraint_free(src);

	return dest;
}

void nfm_constraint_free(nfm_constraint *cst)
{
	isl_pw_qpolynomial_free(cst->constraint);

	free(cst);
}
