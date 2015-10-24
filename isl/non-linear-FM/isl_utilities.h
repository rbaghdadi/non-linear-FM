#ifndef H_ISL_UTILITIES
#define H_ISL_UTILITIES


#if defined(__cplusplus)
extern "C" {
#endif

typedef struct isl_bset_list {
	isl_basic_set *bset;
	struct isl_bset_list *next;
} isl_bset_list;

/* Return the list of basic sets in a set.  */
isl_bset_list *isl_set_get_bsets_list(isl_set *set);

void isl_bset_list_dump(isl_bset_list *list);
void isl_bset_list_free(isl_bset_list *list);

#if defined(__cplusplus)
}
#endif

#endif
