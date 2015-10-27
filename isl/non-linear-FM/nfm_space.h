#ifndef H_NFM_SPACE
#define H_NFM_SPACE

#include <assert.h>
#include <stdio.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* A space describes the name of each variable of a polynome.
   Example: in the polynome 2*X + 3*Y + 5*X*Y + 4*X^2
   the names of the variables are 'X' and 'Y'.
   The name of each variable can only be a character.
*/
typedef struct nfm_space
{
	char *variable_names;
} nfm_space;

/* Create a space where the names
 of the variables are stored in var_names (each charatecter in
 var_names is the name of a variables).
 Example:
 nfm_space_alloc("xyz");
 this creates a space where the name
 of the first variable (dimension) is 'x', the 2nd is 'y' and the 3rd
 is 'z'.
 */
nfm_space *nfm_space_alloc(char *var_names);

void nfm_space_free(nfm_space *space);

void nfm_space_dump(nfm_space *space);

int nfm_space_get_num_vars(nfm_space *space);

/* Return the name of the variable number var_index in the space.
 The index of the first variable is 0.
 */
char nfm_space_get_var_name(nfm_space *space, int var_index);

nfm_space *nfm_space_copy(nfm_space *space);

#if defined(__cplusplus)
}
#endif

#endif
