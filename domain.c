#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <isl/polynomial.h>
#include <isl/map.h>
#include <isl/set.h>
#include <isl/union_set.h>

#include <isl/non-linear-FM/constraint.h>
#include <isl/non-linear-FM/domain.h>
#include <isl/non-linear-FM/isl_interface.h>

#include "debug.h"

isl_space *domain_get_space(isl_ctx *ctx, domain *domain)
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
isl_bset_list *domain_intersect_constraints(isl_ctx *ctx,
		struct domain *domain)
{
	isl_set *intersection;

	assert(domain);

	IF_DEBUG(fprintf(stdout, " Starting intersection function.\n"));

	constraint *constraints_list = domain->constraints_list;
	intersection = isl_set_universe(domain_get_space(ctx, domain));

	IF_DEBUG(fprintf(stdout, " Initial value for intersection is:"));
	IF_DEBUG(isl_set_dump(intersection));

	while (constraints_list != NULL)
	{
		isl_set *set = constraint_set_from_constraint(ctx,
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


/* Compute the union of all the constraints in domain.  */
isl_union_set *domain_union_constraints(isl_ctx *ctx,
		struct domain *domain)
{
	isl_union_set *union_set;

	assert(domain);

	IF_DEBUG(fprintf(stdout, " Starting the union function.\n"));

	constraint *constraints_list = domain->constraints_list;
	union_set = isl_union_set_empty(domain_get_space(ctx, domain));

	IF_DEBUG(fprintf(stdout, " Initial value for the union set is:"));
	IF_DEBUG(isl_union_set_dump(union_set));

	while (constraints_list != NULL)
	{
		isl_set *set = constraint_set_from_constraint(ctx,
				constraints_list);
		IF_DEBUG(fprintf(stdout, " The constraint to compute union"
					 " with, represented as a set:"));
		IF_DEBUG(isl_set_dump(set));

		union_set = isl_union_set_union(union_set,
				isl_union_set_from_set(set));
		IF_DEBUG(fprintf(stdout, " Results of the union:"));
		IF_DEBUG(isl_union_set_dump(union_set));
		constraints_list = constraints_list->next;
	}

	IF_DEBUG(fprintf(stdout, " End of the union function.\n"));

	return union_set;
}


void domain_dump(struct domain *domain)
{
	constraint *head = domain->constraints_list;

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

void domain_free(domain *domain)
{
	assert (domain);

	constraint *head = domain->constraints_list;

	while (head != NULL)
	{
		isl_pw_qpolynomial_free(head->constraint);
		head = head->next;
	}

	free(domain);
}

domain *domain_alloc()
{
	struct domain *domain = (struct domain *)
					malloc(sizeof(struct domain));
	domain->n = 0;
	domain->constraints_list = NULL;

	return domain;
}

/* Add constraint to a domain of quasi-polynomials.  */
void domain_add_constraint(struct domain *domain,
				struct constraint *constraint)
{
	struct constraint *head = domain->constraints_list;

	assert(constraint);

	struct constraint *new_constraint = constraint_copy(constraint);
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
