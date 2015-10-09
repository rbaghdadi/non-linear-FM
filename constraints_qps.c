#include <assert.h>
#include <stdio.h>
#include <isl/polynomial.h>
#include <isl/set.h>
#include <constraints_qps.h>
#include <domain_qps.h>
#include <string.h>
#include <debug.h>

isl_set *constraint_qps_set_from_constraint(isl_ctx *ctx,
		constraint_qps *constraints)
{
	isl_printer *p;

	assert(ctx);
	assert(constraints);

	p = isl_printer_to_str(ctx);

	p = isl_printer_print_pw_qpolynomial(p, constraints->constraint);
	char *str = isl_printer_get_str(p);
	IF_DEBUG(fprintf(stdout, "qp constraint: %s\n", str));

	assert(str);

	/* Translate the qpolynomial into a map.  */
	char *set_str = (char *) malloc((strlen(str)+10)*sizeof(char));
	strcpy(set_str, str);
	IF_DEBUG(fprintf(stdout, "set_str=%s\n", set_str));
	size_t pos_arrow = strcspn(str, ">");
	set_str[pos_arrow-1] = ' ';
	set_str[pos_arrow] = ':';
	IF_DEBUG(fprintf(stdout, "set_str=%s\n", set_str));
	size_t pos_colon = strcspn(&(str[pos_arrow+1]), ":");
	if (strchr(&(str[pos_arrow+1]), ':') != NULL)
	{
		set_str[pos_arrow+1+pos_colon] = ' ';
		IF_DEBUG(fprintf(stdout, "set_str=%s\n", set_str));

		if (constraints->eq == 1)
			strcpy(&(set_str[pos_arrow+1+pos_colon]), "  = 0 and ");
		else
			strcpy(&(set_str[pos_arrow+1+pos_colon]), " >= 0 and ");

		IF_DEBUG(fprintf(stdout, "set_str=%s\n", set_str));
		strncpy(&(set_str[pos_arrow+pos_colon+10]), &(str[pos_arrow+1+pos_colon+1]), strlen(str) - pos_arrow - pos_colon);
		IF_DEBUG(fprintf(stdout, "set_str=%s\n", set_str));
	}
	else
	{
		size_t pos_bracket = strcspn(str, "}");
		set_str[pos_bracket] = ' ';
		if (constraints->eq == 1)
			strcat(&(set_str[pos_bracket]), " = 0 }");
		else
			strcat(&(set_str[pos_bracket]), " >= 0 }");
	}

	IF_DEBUG(fprintf(stdout, "After translating qp -> set: %s\n", set_str));
	isl_set *set = isl_set_read_from_str(ctx, set_str);
	IF_DEBUG(fprintf(stdout, "After importing the set into ISL, the set is:\n"));
	IF_DEBUG(isl_set_dump(set));

	isl_printer_free(p);

	return set;
}


/* A domain described using quasi-polynomial constraints.  */
constraint_qps *constraint_qps_copy(constraint_qps *src)
{
	assert(src);

	constraint_qps *dest = (constraint_qps *)
					malloc(sizeof(constraint_qps));

	dest->constraint = isl_pw_qpolynomial_copy(src->constraint);
	dest->eq = src->eq;

	constraint_qps_free(src);

	return dest;
}

/* Create a quasi-polynomial constraint from an isl_pw_qpolynomial.
   eq is true if the constraint is an equality.
 */
constraint_qps *constraint_qps_from_pw_qpolynomial(isl_pw_qpolynomial *qp,
		unsigned char eq)
{
	assert(qp);

	constraint_qps *cst = (constraint_qps *)
		malloc(sizeof(constraint_qps));

	cst->constraint = qp;
	cst->eq = eq;
	cst->next = NULL;

	return cst;
}
