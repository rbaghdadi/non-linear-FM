#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <isl/set.h>
#include <isl/union_set.h>

#include <isl/non-linear-FM/nfm_constraint.h>
#include <isl/non-linear-FM/nfm_domain.h>
#include <isl/non-linear-FM/nfm_union_domain.h>
#include <isl/non-linear-FM/nfm_isl_interface.h>

#include "isl/non-linear-FM/nfm_debug.h"

void nfm_union_domain_dump(struct nfm_union_domain *union_domain)
{
	nfm_union_domain *head = union_domain;

	fprintf(stdout, "List of domains (%d domains):\n", union_domain->n);

	while (head != NULL)
	{
		if (head->domain != NULL)
		{
			nfm_domain_dump(head->domain);
		}
		else
			fprintf(stdout, "NULL\n");

		head = head->next;
	}
}

void nfm_union_domain_free(nfm_union_domain *union_domain)
{
	assert(union_domain);

	nfm_union_domain *head = union_domain;

	while (head != NULL)
	{
		nfm_domain_free(head->domain);
		head = head->next;
	}

	free(union_domain);
}

nfm_union_domain *nfm_union_domain_alloc()
{
	struct nfm_union_domain *union_domain = (struct nfm_union_domain *)
					malloc(sizeof(struct nfm_union_domain));
	union_domain->n = 0;
	union_domain->domain = NULL;
	union_domain->next = NULL;

	return union_domain;
}

/* Add constraint to a domain of quasi-polynomials.  */
void nfm_union_domain_add_domain(nfm_union_domain *union_domain,
				 nfm_domain *domain)
{
	struct nfm_union_domain *head = union_domain;

	assert(domain);

	if (head != NULL && head->domain != NULL)
	{
		while (head->next != NULL)
			head = head->next;

		head->domain = domain;
		head->next = NULL;
	}
	else
		union_domain->domain = domain;

	union_domain->n++;
}

/* Compute the union of all the constraints in domain.  */
isl_union_set *nfm_union_domain_union_domains(isl_ctx *ctx,
		struct nfm_union_domain *union_domain)
{
	isl_union_set *union_set;

	assert(union_domain);
	assert(union_domain->domain);

	IF_DEBUG(fprintf(stdout, " Starting the union function.\n"));

	nfm_union_domain *head = union_domain;
	union_set = isl_union_set_empty(nfm_domain_get_space(ctx, union_domain->domain));

	IF_DEBUG(fprintf(stdout, " Initial value for the union set is:"));
	IF_DEBUG(isl_union_set_dump(union_set));

	while (head != NULL)
	{
		nfm_domain *domain = head->domain;
		isl_bset_list *bset_list = nfm_domain_intersect_constraints(ctx, domain);
		IF_DEBUG(fprintf(stdout, " The constraints at this iteration,"
					 " represented as a set, are:"));
		IF_DEBUG(isl_bset_list_dump(bset_list));

	  	while (bset_list != NULL)
		{
			union_set = isl_union_set_union(union_set,
					isl_union_set_from_set(
						isl_set_from_basic_set(
							bset_list->bset)));
			bset_list = bset_list->next;
		}

		IF_DEBUG(fprintf(stdout, " Results of the union:"));
		IF_DEBUG(isl_union_set_dump(union_set));
		head  = head->next;
	}

	IF_DEBUG(fprintf(stdout, " End of the union function.\n"));

	return union_set;
}
