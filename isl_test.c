#include <stdio.h>
#include <isl/polynomial.h>
#include <isl/options.h>
#include <isl/set.h>

#include <constraints_qps.h>
#include <domain_qps.h>
#include <isl_utilities.h>

int test_parse(struct isl_ctx *ctx)
{
	const char *str1, *str2, *str3, *str4;
	isl_pw_qpolynomial *qp1, *qp2, *qp3;
	isl_set *s;

	str1 = "{ [i, j] -> i         }";
	str2 = "{ [i, j] -> 2*i - 1   }";
	str3 = "{ [i, j] -> 4*i + 2*j }";
	str4 = "[N,M] -> {[-1 + N, x, y] : M >= N; [-1 + M, x, y] : M <= -1 + N }";

	qp1 = isl_pw_qpolynomial_read_from_str(ctx, str1);
	qp2 = isl_pw_qpolynomial_read_from_str(ctx, str2);
	qp3 = isl_pw_qpolynomial_read_from_str(ctx, str3);
	s = isl_set_read_from_str(ctx, str4);

	fprintf(stdout, "The bset_list of the following set : "); fflush(stdout);
	isl_set_dump(s);
	fprintf(stdout, "is :\n"); fflush(stdout);
	isl_bset_list *list = isl_set_get_bsets_list(s);
	isl_bset_list_dump(list);

	fprintf(stdout, "\n\n"); fflush(stdout);

	domain_qps *domain = domain_qps_alloc();

	constraint_qps *cst1 = constraint_qps_from_pw_qpolynomial(qp1, 0);
	constraint_qps *cst2 = constraint_qps_from_pw_qpolynomial(qp2, 0);
	constraint_qps *cst3 = constraint_qps_from_pw_qpolynomial(qp3, 0);

	domain_qps_add_constraint(domain, cst1);
	domain_qps_add_constraint(domain, cst2);
	domain_qps_add_constraint(domain, cst3);

	fprintf(stdout, "The domain is:\n");
	domain_qps_dump(domain);

	isl_bset_list *intersection = domain_qps_intersect_constraints(ctx, domain);
	fprintf(stdout, "The intersection is:"); fflush(stdout);
	isl_bset_list_dump(intersection); fflush(stdout);
	fprintf(stdout, "\n"); fflush(stdout);

	fprintf(stdout, "\nCalling the union function."); fflush(stdout);
	isl_union_set *union_set = domain_qps_union_constraints(ctx, domain);
	fprintf(stdout, "The union is:\n");
	isl_union_set_dump(union_set);


	isl_set_free(s);
	isl_bset_list_free(list);
	isl_bset_list_free(intersection);
	isl_union_set_free(union_set);
	domain_qps_free(domain);

	return 0;
}


int main(int argc, char **argv)
{
	int i;
	struct isl_ctx *ctx;
	struct isl_options *options;

	options = isl_options_new_with_defaults();
	argc = isl_options_parse(options, argc, argv, ISL_ARG_ALL);
	ctx = isl_ctx_alloc_with_options(&isl_options_args, options);
	test_parse(ctx);
	isl_ctx_free(ctx);
	return 0;
}
