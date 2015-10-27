#include <isl/non-linear-FM/nfm_space.h>
#include <isl/non-linear-FM/nfm_polynome.h>
#include <isl/non-linear-FM/nfm.h>
#include <stdio.h>
#include <stdlib.h>
#include <debug.h>

nfm_term_c *nfm_term_c_alloc(double coeff, int *exponents,
		 char *variable_names)
{
	assert(exponents);
	assert(variable_names);

	nfm_term_c *term = (nfm_term_c *) malloc(sizeof(nfm_term_c));

	term->coeff = coeff;
	term->exponents = exponents;
	term->space = nfm_space_alloc(variable_names);

	return term;
}

void nfm_term_c_free(nfm_term_c *term)
{
	assert(term);

	nfm_space_free(term->space);
	free(term);
}

void nfm_term_c_dump(nfm_term_c *term)
{
	int i;

	if (term == NULL)
	{
		fprintf(stdout, "NULL");
		return;
	}
	
	assert(term->exponents);
	assert(term->space);

	if (term->coeff == 0)
		fprintf(stdout, "0 ");
	else
	{
		fprintf(stdout, "%.1f * ", term->coeff);

		for (i = 0; i < nfm_space_get_num_vars(term->space); i++)
		{
			if (term->exponents[i] != 0)
				fprintf(stdout, "%c^%d ",
					nfm_space_get_var_name(term->space, i),
					term->exponents[i]);
		}
	}
	fflush(stdout);
}

/* Return 1 if the exponents of term1 nad term2 are equal.  */
int nfm_term_c_have_equal_exponents(nfm_term_c *term1, nfm_term_c *term2)
{
	assert(term1);
	assert(term2);
	assert(nfm_space_equal(term1->space, term2->space));

	int i;
	int equal = 1;

	for (i=0; i<nfm_space_get_num_vars(term1->space); i++)
		if (term1->exponents[i] != term2->exponents[i])
			equal = 0;

	return equal;
}

int *nfm_term_c_copy_exponents(nfm_term_c *term)
{
	assert(term);
	assert(term->exponents);

	int i;
	int *exponents =
		(int *) malloc(sizeof(int) *
				nfm_space_get_num_vars(term->space));

	for (i=0; i<nfm_space_get_num_vars(term->space); i++)
		exponents[i] = term->exponents[i];

	return exponents;	
}

/* return the sum of term1 and term2.  */
nfm_term_c *nfm_term_c_add(nfm_keep nfm_term_c *term2, nfm_keep nfm_term_c *term1)
{
	assert(term1);
	assert(term2);
	assert(nfm_space_equal(term1->space, term2->space));
	assert(nfm_term_c_have_equal_exponents(term1, term2));

	nfm_space *space = nfm_space_copy(term1->space);

	nfm_term_c *term3 = nfm_term_c_alloc(term1->coeff + term2->coeff,
			nfm_term_c_copy_exponents(term1),
			space->variable_names);

	return term3;
}

int *nfm_term_c_get_exponent(nfm_term_c *term)
{
	assert(term);

	return term->exponents;
}

/* Return true if exponent0 is identical to exponent1.
 */
int equal_exponents(int size, int *exponent0, int *exponent1)
{
	assert(exponent0);
	assert(exponent1);

	int i;
	int equal = 1;

	for (i=0; i<size; i++)
		if (exponent0[i] != exponent1[i])
			equal = 0;

	return equal;
}

nfm_term_c *nfm_term_c_copy(nfm_term_c *term)
{
	assert(term);

	return nfm_term_c_alloc(term->coeff,
			nfm_term_c_copy_exponents(term),
			term->space->variable_names);
}

/* nfm_polynome_c.
 */

nfm_polynome_c *nfm_polynome_c_alloc()
{
	nfm_polynome_c *polynome = (nfm_polynome_c *)
		malloc(sizeof(nfm_polynome_c));

	polynome->term = NULL;
	polynome->next = NULL;

	return polynome;
}

void nfm_polynome_c_free(nfm_polynome_c *polynome)
{
	assert(polynome);

	nfm_polynome_c *head = polynome;

	while (head != NULL)
	{
		nfm_term_c_free(head->term);
		head = head->next;
	}

	free(polynome);
}

void nfm_polynome_c_add_term_c(nfm_polynome_c *polynome,
		nfm_term_c *term)
{
	assert(term);
	assert(polynome);

	nfm_polynome_c *head = polynome;

	if (term->coeff == 0)
		return;

	if (head->term == NULL)
	{
		head->term = term;
		head->next = NULL;
	}
	else
	{
		assert(nfm_space_equal(nfm_polynome_c_get_space(polynome),
					term->space));

		while ((head->next != NULL) &&
				(!nfm_term_c_have_equal_exponents(head->term, term)))
		{
			head = head->next;
		}

		if ((head->next == NULL) && (!nfm_term_c_have_equal_exponents(head->term, term)))
		{
			nfm_polynome_c *polynome = (nfm_polynome_c *)
				malloc(sizeof(nfm_polynome_c));

			polynome->term = term;
			polynome->next = NULL;

			head->next = polynome;
		}
		else
		{
			head->term = nfm_term_c_add(head->term, term);
		}
	}
}


void nfm_polynome_c_add_new_term_c(nfm_polynome_c *polynome,
		double coeff, int *exponents, char *variable_names)
{
	nfm_term_c *term = nfm_term_c_alloc(coeff, exponents, variable_names);

	nfm_polynome_c_add_term_c(polynome, term);
}

nfm_polynome_c *nfm_polynome_c_copy(nfm_polynome_c *polynome)
{
	assert(polynome);

	nfm_polynome_c *head = polynome;

	nfm_polynome_c *new_polynome = nfm_polynome_c_alloc();
	nfm_polynome_c *new_head = new_polynome;

	while (head != NULL)
	{
		if (head->term != NULL)
			new_head->term = nfm_term_c_copy(head->term);
		else
			new_head->term = NULL;

		if (head->next != NULL)
			new_head->next = nfm_polynome_c_alloc();
		else
			new_head->next = NULL;

		head = head->next;
		new_head = new_head->next;
	}

	return new_polynome;
}

nfm_polynome_c *nfm_polynome_c_add_polynome_c(nfm_polynome_c *polynome1,
		nfm_polynome_c *polynome2)
{
	assert(polynome1);
	assert(polynome2);

	IF_DEBUG(printf("\nComputing the sum of polynome_c1 and Polynome_c2.\n"));
	IF_DEBUG(printf("\npolynome_c1 is:\n"));
	IF_DEBUG(nfm_polynome_c_dump(polynome1));
	IF_DEBUG(printf("\npolynome_c2 is:\n"));
	IF_DEBUG(nfm_polynome_c_dump(polynome2));

	nfm_polynome_c *polynome3 = nfm_polynome_c_copy(polynome2);
	IF_DEBUG(printf("\npolynome3 is a copy of polynome2: "));
	IF_DEBUG(nfm_polynome_c_dump(polynome3));

	nfm_polynome_c *head = polynome1;

	while (head != NULL)
	{
		IF_DEBUG(printf("\nCurrent value of polynome3 : "));
		IF_DEBUG(nfm_polynome_c_dump(polynome3));
		IF_DEBUG(printf("\nAdding the following term of polynome1 into polynome3 : "));
		IF_DEBUG(nfm_term_c_dump(head->term));

		nfm_polynome_c_add_term_c(polynome3, head->term);
		IF_DEBUG(printf("\nAffter the addition, the new value of polynome3 : "));
		IF_DEBUG(nfm_polynome_c_dump(polynome3));

		head = head->next;
	}

	IF_DEBUG(printf("\nFinal value of polynome3 :"));
	IF_DEBUG(nfm_polynome_c_dump(polynome3));
	IF_DEBUG(printf("\n"));

	return polynome3;
}


void nfm_polynome_c_dump(nfm_polynome_c *polynome)
{
	assert(polynome);

	nfm_polynome_c *head = polynome;

	while (head != NULL)
	{
		if (head->term != NULL)
			nfm_term_c_dump(head->term);
		else
			fprintf(stdout, "NULL");

		if (head->next != NULL)
			fprintf(stdout, "+ ");
		head = head->next;
	}
}

nfm_space *nfm_polynome_c_get_space(nfm_polynome_c *polynome)
{
	assert(polynome);
	assert(polynome->term);

	return polynome->term->space;
}

nfm_term_c *nfm_polynome_c_get_term_c_by_exponent(nfm_polynome_c *polynome,
		int *exponent)
{
	assert(polynome);
	assert(exponent);

	nfm_polynome_c *head = polynome;

	while (head != NULL)
	{
		if (equal_exponents(nfm_space_get_num_vars(head->term->space),
				nfm_term_c_get_exponent(head->term), exponent))
			return head->term;
		else
			head = head->next;
	}

	return NULL;
}


/* nfm_term_p.
 */

nfm_term_p *nfm_term_p_alloc(double coeff_inner, int *exponents_inner,
		 char *variable_names_inner, int *exponents_outer,
		 char *variable_names_outer)
{
	assert(exponents_inner);
	assert(variable_names_inner);
	assert(exponents_outer);
	assert(variable_names_outer);

	nfm_polynome_c *poly_c = nfm_polynome_c_alloc();
	nfm_term_c *term_c = nfm_term_c_alloc(coeff_inner, exponents_inner,
		       variable_names_inner);
	nfm_polynome_c_add_term_c(poly_c, term_c);

	nfm_term_p *term_p = (nfm_term_p *) malloc(sizeof(nfm_term_p));

	term_p->coeff = poly_c;
	term_p->exponents = exponents_outer;
	term_p->space = nfm_space_alloc(variable_names_outer);

	return term_p;
}

nfm_term_p *nfm_term_p_alloc_using_polynome_c(nfm_polynome_c *polynome_inner,
		int *exponents_outer, char *variable_names_outer)
{
	assert(polynome_inner);
	assert(exponents_outer);
	assert(variable_names_outer);

	nfm_term_p *term_p = (nfm_term_p *) malloc(sizeof(nfm_term_p));

	term_p->coeff = polynome_inner;
	term_p->exponents = exponents_outer;
	term_p->space = nfm_space_alloc(variable_names_outer);

	return term_p;
}

void nfm_term_p_free(nfm_term_p *term)
{
	assert(term);

	nfm_space_free(term->space);
	nfm_polynome_c_free(term->coeff);
	free(term);
}

void nfm_term_p_dump(nfm_term_p *term)
{
	assert(term);
	assert(term->exponents);
	assert(term->space);

	int i;

	if (term->coeff->term->coeff == 0)
		fprintf(stdout, "0 ");
	else
	{
		fprintf(stdout, "(");
		nfm_polynome_c_dump(term->coeff);
		fprintf(stdout, ") * ");

		for (i = 0; i < nfm_space_get_num_vars(term->space); i++)
		{
			if (term->exponents[i] != 0)
				fprintf(stdout, "%c^%d ",
					nfm_space_get_var_name(term->space, i),
					term->exponents[i]);
		}
	}
	fflush(stdout);
}

/* Return 1 if the exponents of term1 nad term2 are equal.  */
int nfm_term_p_equal_exponents(nfm_term_p *term1, nfm_term_p *term2)
{
	assert(term1);
	assert(term2);
	assert(nfm_space_equal(term1->space, term2->space));

	int i;
	int equal = 1;

	for (i=0; i<nfm_space_get_num_vars(term1->space); i++)
		if (term1->exponents[i] != term2->exponents[i])
			equal = 0;

	return equal;
}

int *nfm_term_p_copy_exponents(nfm_term_p *term)
{
	assert(term);
	assert(term->exponents);

	int i;
	int *exponents =
		(int *) malloc(sizeof(int) *
				nfm_space_get_num_vars(term->space));

	for (i=0; i<nfm_space_get_num_vars(term->space); i++)
		exponents[i] = term->exponents[i];

	return exponents;	
}


/* return the sum of term1 and term2.  */
nfm_term_p *nfm_term_p_add_term_p(nfm_keep nfm_term_p *term_p1,
		nfm_keep nfm_term_p *term_p2)
{
	assert(term_p1);
	assert(term_p2);
	assert(nfm_space_equal(term_p1->space, term_p2->space));
	assert(nfm_term_p_equal_exponents(term_p1, term_p2));

	nfm_space *space_inner = nfm_space_copy(term_p1->coeff->term->space);
	nfm_space *space_outer = nfm_space_copy(term_p1->space);

	nfm_polynome_c *polynome_c1 = term_p1->coeff;
	nfm_polynome_c *polynome_c2 = term_p2->coeff;
	nfm_polynome_c *polynome_c3 =
		nfm_polynome_c_add_polynome_c(polynome_c1, polynome_c2);

	nfm_term_p *term_p3 = nfm_term_p_alloc_using_polynome_c(polynome_c3,
			term_p1->exponents, term_p1->space->variable_names);

	return term_p3;
}
