#include <stdio.h>
#include <isl/polynomial.h>
#include <isl/options.h>
#include <isl/set.h>
#include <domain_qps.h>
#include <constraints_qps.h>

int test_parse(struct isl_ctx *ctx)
{
	const char *str1, *str2, *str3;
	isl_pw_qpolynomial *qp1, *qp2, *qp3;

	str1 = "{ [i, j] -> i         }";
	str2 = "{ [i, j] -> 2*i - 1   }";
	str3 = "{ [i, j] -> 4*i + 2*j }";

	qp1 = isl_pw_qpolynomial_read_from_str(ctx, str1);
	qp2 = isl_pw_qpolynomial_read_from_str(ctx, str2);
	qp3 = isl_pw_qpolynomial_read_from_str(ctx, str3);

	domain_qps *domain = domain_qps_alloc();

	constraint_qps *cst1 = constraint_qps_from_pw_qpolynomial(qp1, 0);
	constraint_qps *cst2 = constraint_qps_from_pw_qpolynomial(qp2, 0);
	constraint_qps *cst3 = constraint_qps_from_pw_qpolynomial(qp3, 0);

	domain_qps_add_constraint(domain, cst1);
	domain_qps_add_constraint(domain, cst2);
	domain_qps_add_constraint(domain, cst3);

	fprintf(stdout, "The domain is:\n");
	domain_qps_dump(domain);

	isl_set *intersection = domain_qps_intersect_constraints(ctx, domain);
	fprintf(stdout, "The intersection is:\n");
	isl_set_dump(intersection);

	isl_union_set *union_set = domain_qps_union_constraints(ctx, domain);
	fprintf(stdout, "The union is:\n");
	isl_union_set_dump(union_set);


	isl_set_free(intersection);
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
