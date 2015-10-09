#include <assert.h>
#include <stdio.h>
#include <isl/polynomial.h>
#include <isl/map.h>
#include <isl/set.h>
#include <constraints_qps.h>
#include <domain_qps.h>
#include <string.h>
#include <debug.h>

isl_space *domain_qps_get_space(isl_ctx *ctx, domain_qps *domain)
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
isl_set *domain_qps_intersect_constraints(isl_ctx *ctx,
		struct domain_qps *domain)
{
	int i;
	isl_set *intersection;

	assert(domain);

	constraint_qps *constraints_list = domain->constraints_list;
	intersection = isl_set_universe(domain_qps_get_space(ctx, domain));

	IF_DEBUG(fprintf(stdout, "Initial value for intersection is:\n"));
	IF_DEBUG(isl_set_dump(intersection));
	IF_DEBUG(fprintf(stdout, "\n"));

	while (constraints_list != NULL)
	{
		isl_set *set = constraint_qps_set_from_constraint(ctx,
				constraints_list);
		IF_DEBUG(fprintf(stdout, "constraint represented as a set: \n"));
		IF_DEBUG(isl_set_dump(set));

		intersection = isl_set_intersect(intersection,
				set);
		IF_DEBUG(fprintf(stdout, "New value for intersection is:\n"));
		IF_DEBUG(isl_set_dump(intersection));
		constraints_list = constraints_list->next;
	}

	return intersection;
}

void constraint_qps_free(constraint_qps *cst)
{
	isl_pw_qpolynomial_free(cst->constraint);

	free(cst);
}

void domain_qps_dump(struct domain_qps *domain)
{
	constraint_qps *head = domain->constraints_list;

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

void domain_qps_free(domain_qps *domain)
{
	assert (domain);

	constraint_qps *head = domain->constraints_list;

	while (head != NULL)
	{
		isl_pw_qpolynomial_free(head->constraint);
		head = head->next;
	}

	free(domain);
}

domain_qps *domain_qps_alloc()
{
	struct domain_qps *domain = (struct domain_qps *)
					malloc(sizeof(struct domain_qps));
	domain->n = 0;
	domain->constraints_list = NULL;

	return domain;
}

/* Add constraint to a domain of quasi-polynomials.  */
void domain_qps_add_constraint(struct domain_qps *domain,
				struct constraint_qps *constraint)
{
	constraint_qps *head = domain->constraints_list;

	assert(constraint);

	constraint_qps *new_constraint = constraint_qps_copy(constraint);
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
