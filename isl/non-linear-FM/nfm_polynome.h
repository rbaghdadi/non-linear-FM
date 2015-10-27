#ifndef H_NFM_POLYNOME_C
#define H_NFM_POLYNOME_C

#include <assert.h>
#include <stdio.h>
#include <isl/non-linear-FM/nfm_space.h>
#include <isl/non-linear-FM/nfm.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* A term of a multivariate polynome.  Each coefficient in this term
   is a constant.
   Examples of terms: 4*A, 2*A^2, 2, 12*A*B
*/
typedef struct nfm_term_c
{
	double coeff;	   /* The coefficient of this term.  */
	int *exponents;    /* An array representing the exponent of each
			      variable in the space of the term.  */
	nfm_space *space;  /* The space of the term.  */
} nfm_term_c;


/* A term of a multivariate polynome.  Each coefficient in this term
   is a multivariate polynome by itself.
   Examples of terms: 4*A*X^2, (A+B)*X*Y, 2, 2*X*Y^2
*/
typedef struct nfm_term_p
{
	struct nfm_polynome_c *coeff;
	int *exponents;
	nfm_space *space;
} nfm_term_p;


/* A multivariate polynome.  Each coefficient of this polynome
   is a constant.
*/
typedef struct nfm_polynome_c
{
	struct nfm_term_c *term;
	struct nfm_polynome_c *next;
} nfm_polynome_c;


// A multivariate polynome.  Each coefficient of this polynome
// is a multivariate polynome also.
typedef struct nfm_polynome_p
{
	nfm_term_p *term;
	struct nfm_polynome_p *next;
} nfm_polynome_p;


/* Allocate a term for an nfm_polynome_c (i.e. a polynome that has constant
   coefficients).
 */
nfm_term_c *nfm_term_c_alloc(double coeff, int *exponents,
		 char *variable_names);

void nfm_term_c_free(nfm_term_c *term);

void nfm_term_c_dump(nfm_term_c *term);

nfm_term_c *nfm_term_c_add(nfm_keep nfm_term_c *term2, nfm_keep nfm_term_c *term1);

/* nfm_polynome_c.
 */

nfm_polynome_c *nfm_polynome_c_alloc();

nfm_term_p *nfm_term_p_alloc_using_polynome_c(nfm_polynome_c *polynome_inner,
		int *exponents_outer, char *variable_names_outer);

void nfm_polynome_c_add_term_c(nfm_polynome_c *polynome,
		nfm_term_c *term);

void nfm_polynome_c_add_new_term_c(nfm_polynome_c *polynome,
		double coeff, int *exponents, char *variable_names);

void nfm_polynome_c_dump(nfm_polynome_c *polynome);

nfm_space *nfm_polynome_c_get_space(nfm_polynome_c *polynome);

/* nfm_term_p.
 */

nfm_term_p *nfm_term_p_alloc(double coeff_inner, int *exponents_inner,
		 char *variable_names_inner, int *exponents_outer,
		 char *variable_names_outer);

void nfm_term_p_free(nfm_term_p *term);

void nfm_term_p_dump(nfm_term_p *term);

int nfm_term_p_equal_exponents(nfm_term_p *term1, nfm_term_p *term2);

int *nfm_term_p_copy_exponents(nfm_term_p *term);

nfm_term_p *nfm_term_p_add_term_p(nfm_keep nfm_term_p *term2, nfm_keep nfm_term_p *term1);

#if defined(__cplusplus)
}
#endif

#endif
