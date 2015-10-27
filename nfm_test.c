#include <stdio.h>
#include <isl/polynomial.h>
#include <isl/options.h>
#include <isl/set.h>

#include <nfm_constraint.h>
#include <nfm_domain.h>
#include <nfm_isl_interface.h>
#include <nfm_union_domain.h>
#include <nfm_polynome.h>

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

	fprintf(stdout, "Allocating terms.\n"); fflush(stdout);

	int e0[3] = {2,1,0};
	nfm_term_c *term0 = nfm_term_c_alloc(5, e0, "xyz");
	int e1[3] = {0,2,0};
	nfm_term_c *term1 = nfm_term_c_alloc(0, e1, "xyz");
	int e2[3] = {2,1,0};
	nfm_term_c *term2 = nfm_term_c_alloc(-3.5, e2, "xyz");

	fprintf(stdout, "Dumping terms.\n"); fflush(stdout);

	fprintf(stdout, "term 1: "); fflush(stdout);
	nfm_term_c_dump(term0);

	fprintf(stdout, "\n"); fflush(stdout);

	fprintf(stdout, "term 2: "); fflush(stdout);
	nfm_term_c_dump(term1);

	fprintf(stdout, "\n\n"); fflush(stdout);

	fprintf(stdout, "Adding two terms:\n"); fflush(stdout);
	nfm_term_c_dump(term0);

	fprintf(stdout, "\nand\n"); fflush(stdout);
	nfm_term_c_dump(term2);

	nfm_term_c *term4 = nfm_term_c_add(term0, term2);
	fprintf(stdout, "\nNew term:\n"); fflush(stdout);
	nfm_term_c_dump(term4);

	fprintf(stdout, "\n\n"); fflush(stdout);

	nfm_polynome_c *polynome = nfm_polynome_c_alloc();
	nfm_polynome_c_add_term_c(polynome, term0);
	nfm_polynome_c_add_term_c(polynome, term1);

	int e3[3] = {2,0,0};
	nfm_polynome_c_add_new_term_c(polynome, -2.5, e3, "xyz");

	fprintf(stdout, "Terms were added\n"); fflush(stdout);

	nfm_polynome_c_dump(polynome);
	
	fprintf(stdout, "\nAdding a new term  -2.5 * x^2\n"); fflush(stdout);

	int e4[3] = {2,0,0};
	nfm_polynome_c_add_new_term_c(polynome, -2.5, e4, "xyz");

	fprintf(stdout, "After adding the new term\n"); fflush(stdout);
	nfm_polynome_c_dump(polynome);

	nfm_term_c_free(term0);
	nfm_term_c_free(term1);

	fprintf(stdout, "\n\n"); fflush(stdout);


	fprintf(stdout, "Allocating term_ps.\n"); fflush(stdout);

	int exp_in0[2] = {1,0};
	int exp_out0[2] = {1,0};
	nfm_term_p *term_p0 = nfm_term_p_alloc(2, exp_in0, "AB", exp_out0, "XY");

	int exp_in1[2] = {0,1};
	int exp_out1[2] = {0,1};
	nfm_polynome_c *polynome_c1 = nfm_polynome_c_alloc();
	nfm_polynome_c_add_new_term_c(polynome_c1, -6, exp_in1,"AB");
	nfm_term_p *term_p1 = nfm_term_p_alloc_using_polynome_c(polynome_c1, exp_out1, "XY");

	int exp_in2[2] = {0,1};
	int exp_out2[2] = {0,1};
	nfm_polynome_c *polynome_c2 = nfm_polynome_c_alloc();
	nfm_polynome_c_add_new_term_c(polynome_c2, 7, exp_in2,"AB");
	nfm_term_p *term_p2 = nfm_term_p_alloc_using_polynome_c(polynome_c2, exp_out2, "XY");

	int exp_in25[2] = {1,0};
	int exp_out25[2] = {0,1};
	nfm_polynome_c *polynome_c25 = nfm_polynome_c_alloc();
	nfm_polynome_c_add_new_term_c(polynome_c25, 13, exp_in25,"AB");
	nfm_term_p *term_p25 = nfm_term_p_alloc_using_polynome_c(polynome_c25, exp_out25, "XY");

	fprintf(stdout, "Dumping termp_p0, term_p1, term_p2 and term_p25.\n"); fflush(stdout);
	nfm_term_p_dump(term_p0);
	fprintf(stdout, "\n"); fflush(stdout);
	nfm_term_p_dump(term_p1);
	fprintf(stdout, "\n"); fflush(stdout);
	nfm_term_p_dump(term_p2);
	fprintf(stdout, "\n"); fflush(stdout);
	nfm_term_p_dump(term_p25);

	fprintf(stdout, "\n\n"); fflush(stdout);

	fprintf(stdout, "Computing the sum of term_p1 and term_p2:\n"); fflush(stdout);
	nfm_term_p *term_p3 = nfm_term_p_add_term_p(term_p1, term_p2);
	fprintf(stdout, "Dumping the sum:\n"); fflush(stdout);
	nfm_term_p_dump(term_p3);

	fprintf(stdout, "\nComputing the sum of term_p25 and term_p2:\n"); fflush(stdout);
	nfm_term_p *term_p4 = nfm_term_p_add_term_p(term_p25, term_p2);
	fprintf(stdout, "Dumping the sum:\n"); fflush(stdout);
	nfm_term_p_dump(term_p4);fflush(stdout);


	nfm_term_p_free(term_p0);
	nfm_term_p_free(term_p1);
	nfm_term_p_free(term_p2);
	nfm_term_p_free(term_p3);
	nfm_term_p_free(term_p4);

	fprintf(stdout, "\n\n"); fflush(stdout);

	nfm_domain *domain = nfm_domain_alloc();

	nfm_constraint *cst1 = nfm_constraint_from_pw_qpolynomial(qp1, 0);
	nfm_constraint *cst2 = nfm_constraint_from_pw_qpolynomial(qp2, 0);
	nfm_constraint *cst3 = nfm_constraint_from_pw_qpolynomial(qp3, 0);

	nfm_domain_add_constraint(domain, cst1);
	nfm_domain_add_constraint(domain, cst2);
	nfm_domain_add_constraint(domain, cst3);

	fprintf(stdout, "The domain is:\n");
	nfm_domain_dump(domain);

	isl_bset_list *intersection = nfm_domain_intersect_constraints(ctx, domain);
	fprintf(stdout, "The intersection is:"); fflush(stdout);
	isl_bset_list_dump(intersection); fflush(stdout);
	fprintf(stdout, "\n"); fflush(stdout);

	nfm_union_domain *union_domain = nfm_union_domain_alloc();
	nfm_union_domain_add_domain(union_domain, domain);
	fprintf(stdout, "\nThe union domain is:"); fflush(stdout);
	nfm_union_domain_dump(union_domain);
	fprintf(stdout, "\nCalling the union function."); fflush(stdout);
	isl_union_set *union_set = nfm_union_domain_union_domains(ctx, union_domain);
	fprintf(stdout, "The union is:\n");
	isl_union_set_dump(union_set);

	isl_set_free(s);
	isl_bset_list_free(list);
	isl_bset_list_free(intersection);
	isl_union_set_free(union_set);
	nfm_domain_free(domain);

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
