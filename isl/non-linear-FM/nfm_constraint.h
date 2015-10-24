#ifndef H_CONSTRAINTS_GPS
#define H_CONSTRAINTS_GPS

#include <assert.h>
#include <stdio.h>
#include <isl/polynomial.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct nfm_constraint
{
	isl_pw_qpolynomial *constraint;
	unsigned char eq;
	struct nfm_constraint *next;
} nfm_constraint;

void nfm_constraint_free(nfm_constraint *cst);

/* A domain described using quasi-polynomial constraints.  */
nfm_constraint *nfm_constraint_copy(nfm_constraint *src);

#if defined(__cplusplus)
}
#endif

#endif
