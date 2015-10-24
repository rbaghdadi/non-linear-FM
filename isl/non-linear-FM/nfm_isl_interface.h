#ifndef H_ISL_UTILITIES
#define H_ISL_UTILITIES

#include <isl/set.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct isl_bset_list {
	isl_basic_set *bset;
	struct isl_bset_list *next;
} isl_bset_list;

/* Create an ISL set from a constraint.  */
isl_set *nfm_constraint_from_set(isl_ctx *ctx,
		nfm_constraint *constraints);

/* Create a quasi-polynomial constraint from an isl_pw_qpolynomial.  */
nfm_constraint *nfm_constraint_from_pw_qpolynomial(isl_pw_qpolynomial *qp,
		unsigned char eq);

/* Return the list of basic sets in a set.  */
isl_bset_list *isl_set_get_bsets_list(isl_set *set);

void isl_bset_list_dump(isl_bset_list *list);
void isl_bset_list_free(isl_bset_list *list);

#if defined(__cplusplus)
}
#endif

#endif
