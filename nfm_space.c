#include <isl/non-linear-FM/nfm_space.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

nfm_space *nfm_space_alloc(char *var_names)
{
	assert(var_names);

	nfm_space *space = (nfm_space *) malloc(sizeof(struct nfm_space));

	space->variable_names = var_names;
}

void nfm_space_free(nfm_space *space)
{
	assert(space);
	assert(space->variable_names);

	free(space);
}

void nfm_space_dump(nfm_space *space)
{
	assert(space);
	assert(space->variable_names);

	fprintf(stdout, "The variables of the space are:%s",
			space->variable_names);
}

int nfm_space_get_num_vars(nfm_space *space)
{
	assert(space);
	assert(space->variable_names);
	
	return strlen(space->variable_names);
}

char nfm_space_get_var_name(nfm_space *space, int index)
{
	assert(space);
	assert(space->variable_names);

	if ((index < 0) || (index >= nfm_space_get_num_vars(space)))
	{
		fprintf(stderr, "Error, index out of range\n");
		assert(0);
	}

	return space->variable_names[index];
}

/* Return 1 if the two spaces are equal and 0 otherwise.  */
int nfm_space_equal(nfm_space *sp1, nfm_space *sp2)
{
	assert(sp1);
	assert(sp2);

	if (strcmp(sp1->variable_names, sp2->variable_names) == 0)
		return 1;
	else
		return 0;	
}

nfm_space *nfm_space_copy(nfm_space *space)
{
	assert(space);

	char *str =
		(char *) malloc(sizeof(char)*nfm_space_get_num_vars(space));

	nfm_space *new_space = nfm_space_alloc(str);
	strcpy(str, space->variable_names);
	new_space->variable_names = str;

	return new_space;
}
