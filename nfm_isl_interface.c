#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <isl/set.h>
#include <isl/union_set.h>

#include <isl/non-linear-FM/nfm_constraint.h>
#include <isl/non-linear-FM/nfm_domain.h>
#include <isl/non-linear-FM/nfm_isl_interface.h>

#include "isl/non-linear-FM/nfm_debug.h"

/* Create a constraint from an isl_pw_qpolynomial.
   eq is true if the constraint is an equality.
 */
nfm_constraint *nfm_constraint_from_pw_qpolynomial(isl_pw_qpolynomial *qp,
		unsigned char eq)
{
	assert(qp);

	nfm_constraint *cst = (nfm_constraint *)
		malloc(sizeof(nfm_constraint));

	cst->constraint = qp;
	cst->eq = eq;
	cst->next = NULL;

	return cst;
}

isl_set *nfm_constraint_from_set(isl_ctx *ctx,
		nfm_constraint *constraints)
{
	isl_printer *p;

	assert(ctx);
	assert(constraints);

	IF_DEBUG(fprintf(stdout, "  Transforming a constraint into ISL set.\n"));

	p = isl_printer_to_str(ctx);

	p = isl_printer_print_pw_qpolynomial(p, constraints->constraint);
	char *str = isl_printer_get_str(p);
	IF_DEBUG(fprintf(stdout, "  The input Qpolynomianl constraint: %s\n", str));

	assert(str);

	/* Translate the qpolynomial into a map.  */
	char *set_str = (char *) malloc((strlen(str)+10)*sizeof(char));
	strcpy(set_str, str);
	IF_DEBUG2(fprintf(stdout, "  set_str=%s\n", set_str));
	size_t pos_arrow = strcspn(str, ">");
	set_str[pos_arrow-1] = ' ';
	set_str[pos_arrow] = ':';
	IF_DEBUG2(fprintf(stdout, "  set_str=%s\n", set_str));
	size_t pos_colon = strcspn(&(str[pos_arrow+1]), ":");
	if (strchr(&(str[pos_arrow+1]), ':') != NULL)
	{
		set_str[pos_arrow+1+pos_colon] = ' ';
		IF_DEBUG2(fprintf(stdout, "  set_str=%s\n", set_str));

		if (constraints->eq == 1)
			strcpy(&(set_str[pos_arrow+1+pos_colon]), "  = 0 and ");
		else
			strcpy(&(set_str[pos_arrow+1+pos_colon]), " >= 0 and ");

		IF_DEBUG2(fprintf(stdout, "  set_str=%s\n", set_str));
		strncpy(&(set_str[pos_arrow+pos_colon+10]), &(str[pos_arrow+1+pos_colon+1]), strlen(str) - pos_arrow - pos_colon);
		IF_DEBUG2(fprintf(stdout, "  set_str=%s\n", set_str));
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

	IF_DEBUG(fprintf(stdout, "  The Qpolynomial translated into a set is: %s\n", set_str));
	isl_set *set = isl_set_read_from_str(ctx, set_str);

	isl_printer_free(p);

	return set;
}

isl_stat extract_basic_set_from_set(isl_basic_set *bset, void *bset_list)
{
	isl_bset_list **list = (isl_bset_list **) bset_list;

	if (*list == NULL)
	{
		isl_bset_list *node = (isl_bset_list *)
			malloc(sizeof(isl_bset_list));

		node->bset = bset;
		node->next = NULL;

		*list = node;
	}
	else
	{

		while((*list)->next != NULL)
			(*list) = (*list)->next;

		isl_bset_list *node = (isl_bset_list *)
			malloc(sizeof(isl_bset_list));

		node->bset = bset;
		node->next = NULL;

		(*list)->next = node;
	}

	return isl_stat_ok;
}

/* Return the list of basic sets in a set.  */
isl_bset_list *isl_set_get_bsets_list(isl_set *set)
{
	assert(set);

	isl_bset_list *list = NULL;

	isl_set_foreach_basic_set(set,
			extract_basic_set_from_set, &list);

	return list;
}

void isl_bset_list_dump(isl_bset_list *list)
{
	isl_bset_list *node = list;

	while (node != NULL)
	{
		isl_basic_set_dump(node->bset);
		node = node->next;
	}
}

void isl_bset_list_free(isl_bset_list *list)
{
	isl_bset_list *node = list;
	isl_bset_list *next_node;

	while (node != NULL)
	{
		isl_basic_set_free(node->bset);
		next_node = node->next;
		free(node);
		node = next_node;
	}
}
