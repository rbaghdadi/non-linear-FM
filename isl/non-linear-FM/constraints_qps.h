#ifndef H_CONSTRAINTS_GPS
#define H_CONSTRAINTS_GPS

#include <assert.h>
#include <stdio.h>
#include <isl/polynomial.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct constraint_qps
{
	isl_pw_qpolynomial *constraint;
	unsigned char eq;
	struct constraint_qps *next;
} constraint_qps;

/* Create an ISL set from a constraint.  */
isl_set *constraint_qps_set_from_constraint(isl_ctx *ctx,
		constraint_qps *constraints);

/* A domain described using quasi-polynomial constraints.  */
constraint_qps *constraint_qps_copy(constraint_qps *src);

/* Create a quasi-polynomial constraint from an isl_pw_qpolynomial.  */
constraint_qps *constraint_qps_from_pw_qpolynomial(isl_pw_qpolynomial *qp,
		unsigned char eq);

#if defined(__cplusplus)
}
#endif

#endif
