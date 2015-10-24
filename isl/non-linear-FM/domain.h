#ifndef H_DOMAIN_QPS
#define H_DOMAIN_QPS

#include <isl/polynomial.h>
#include <isl/non-linear-FM/constraint.h>
#include <isl/non-linear-FM/isl_interface.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct domain
{
	int n;
	constraint *constraints_list;
} domain;

isl_space *domain_get_space(isl_ctx *ctx, domain *domain);
isl_bset_list *domain_intersect_constraints(isl_ctx *ctx,
		struct domain *domain);
isl_union_set *domain_union_constraints(isl_ctx *ctx,
		struct domain *domain);
void constraint_free(constraint *cst);
void domain_dump(struct domain *domain);

void domain_free(domain *domain);
domain *domain_alloc();

/* Add constraint to a domain of quasi-polynomials.  */
void domain_add_constraint(struct domain *domain,
				struct constraint *constraint);

isl_bset_list *isl_set_get_bsets_list(isl_set *set);

#if defined(__cplusplus)
}
#endif

#endif
