#ifndef H_DOMAIN_QPS
#define H_DOMAIN_QPS

#include <isl/polynomial.h>
#include <isl/non-linear-FM/nfm_constraint.h>
#include <isl/non-linear-FM/nfm_isl_interface.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct nfm_domain
{
	int n;
	nfm_constraint *constraints_list;
} nfm_domain;

isl_space *nfm_domain_get_space(isl_ctx *ctx, nfm_domain *domain);
isl_bset_list *nfm_domain_intersect_constraints(isl_ctx *ctx,
		struct nfm_domain *domain);
isl_union_set *nfm_domain_union_constraints(isl_ctx *ctx,
		struct nfm_domain *domain);
void nfm_domain_dump(struct nfm_domain *domain);

void nfm_domain_free(nfm_domain *domain);
nfm_domain *nfm_domain_alloc();

/* Add constraint to a domain of quasi-polynomials.  */
void nfm_domain_add_constraint(struct nfm_domain *domain,
				struct nfm_constraint *constraint);

isl_bset_list *isl_set_get_bsets_list(isl_set *set);

#if defined(__cplusplus)
}
#endif

#endif
