#ifndef H_UNION_DOMAIN_
#define H_UNION_DOMAIN_

#include <isl/non-linear-FM/nfm_domain.h>
#include <isl/non-linear-FM/nfm_isl_interface.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct nfm_union_domain
{
	int n;
	nfm_domain *domain;
	struct nfm_union_domain *next;
} nfm_union_domain;

isl_union_set *nfm_union_domain_union_domains(isl_ctx *ctx,
		struct nfm_union_domain *union_domain);
nfm_union_domain *nfm_union_domain_alloc();
void nfm_union_domain_dump(struct nfm_union_domain *union_domain);
void nfm_union_domain_free(nfm_union_domain *union_domain);
void nfm_union_domain_add_domain(nfm_union_domain *union_domain,
				 nfm_domain *domain);

#if defined(__cplusplus)
}
#endif

#endif
