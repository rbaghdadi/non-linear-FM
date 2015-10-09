#ifndef H_DOMAIN_QPS
#define H_DOMAIN_QPS

#include <isl/polynomial.h>
#include <constraints_qps.h>

typedef struct domain_qps
{
	int n;
	constraint_qps *constraints_list;
} domain_qps;

isl_space *domain_qps_get_space(isl_ctx *ctx, domain_qps *domain);
isl_set *domain_qps_intersect_constraints(isl_ctx *ctx,
		struct domain_qps *domain);
void constraint_qps_free(constraint_qps *cst);
void domain_qps_dump(struct domain_qps *domain);

void domain_qps_free(domain_qps *domain);
domain_qps *domain_qps_alloc();

/* Add constraint to a domain of quasi-polynomials.  */
void domain_qps_add_constraint(struct domain_qps *domain,
				struct constraint_qps *constraint);
#endif
