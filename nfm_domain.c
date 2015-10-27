#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <isl/polynomial.h>
#include <isl/map.h>
#include <isl/set.h>
#include <isl/union_set.h>

#include <isl/non-linear-FM/nfm_constraint.h>
#include <isl/non-linear-FM/nfm_domain.h>
#include <isl/non-linear-FM/nfm_isl_interface.h>

#include "isl/non-linear-FM/nfm_debug.h"

isl_space *nfm_domain_get_space(isl_ctx *ctx, nfm_domain *domain)
{
	assert(domain);

	if (domain->constraints_list != NULL)
	{
		isl_space *qpspace =
			isl_pw_qpolynomial_get_domain_space(
					domain->constraints_list->constraint);
		return qpspace;
	}
	else
		return NULL;
}

/* Compute the intersection of all the constraints in domain.  */
isl_bset_list *nfm_domain_intersect_constraints(isl_ctx *ctx,
		struct nfm_domain *domain)
{
	isl_set *intersection;

	assert(domain);

	IF_DEBUG(fprintf(stdout, " Starting intersection function.\n"));

	nfm_constraint *constraints_list = domain->constraints_list;
	intersection = isl_set_universe(nfm_domain_get_space(ctx, domain));

	IF_DEBUG(fprintf(stdout, " Initial value for intersection is:"));
	IF_DEBUG(isl_set_dump(intersection));

	while (constraints_list != NULL)
	{
		isl_set *set = nfm_constraint_from_set(ctx,
				constraints_list);
		IF_DEBUG(fprintf(stdout, " Constraint to intersect with,"
					"represented as a set:"));
		IF_DEBUG(isl_set_dump(set));

		intersection = isl_set_intersect(intersection,
				set);
		IF_DEBUG(fprintf(stdout, " Result of intersection is:"));
		IF_DEBUG(isl_set_dump(intersection));
		constraints_list = constraints_list->next;
	}

	IF_DEBUG(fprintf(stdout, " End of intersection function.\n"));

	isl_bset_list *intersection_list =
		isl_set_get_bsets_list(intersection);

	isl_set_free(intersection);

	return intersection_list;
}


void nfm_domain_dump(struct nfm_domain *domain)
{
	nfm_constraint *head = domain->constraints_list;

	fprintf(stdout, "List of constraints (%d constraints):\n", domain->n);

	while (head != NULL)
	{
		if (head->constraint != NULL)
		{
			fprintf(stdout, head->eq?"eq: ":"ineq: ");
			fflush(stdout);
			isl_pw_qpolynomial_dump(head->constraint);
		}
		else
			fprintf(stdout, "NULL\n");

		head = head->next;
	}
}

void nfm_domain_free(nfm_domain *domain)
{
	assert(domain);

	nfm_constraint *head = domain->constraints_list;

	while (head != NULL)
	{
		isl_pw_qpolynomial_free(head->constraint);
		head = head->next;
	}

	free(domain);
}

nfm_domain *nfm_domain_alloc()
{
	struct nfm_domain *domain = (struct nfm_domain *)
					malloc(sizeof(struct nfm_domain));
	domain->n = 0;
	domain->constraints_list = NULL;

	return domain;
}

/* Add constraint to a domain of quasi-polynomials.  */
void nfm_domain_add_constraint(struct nfm_domain *domain,
				struct nfm_constraint *constraint)
{
	struct nfm_constraint *head = domain->constraints_list;

	assert(constraint);

	struct nfm_constraint *new_constraint = nfm_constraint_copy(constraint);
	new_constraint->next = NULL;

	if (head != NULL)
	{
		while (head->next != NULL)
			head = head->next;

		head->next = new_constraint;
	}
	else
		domain->constraints_list = new_constraint;

	domain->n++;
}
