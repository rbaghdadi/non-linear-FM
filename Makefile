HEADER_DIR=-I../include/ -I. -Iisl/non-linear-FM/ -Ipolynomial_lib/
LIB_DIR=-L../


all:
	gcc -c ${HEADER_DIR} ${LIB_DIR} isl_test.c -o isl_test.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} isl_utilities.c -o isl_utilities.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} constraints_qps.c -o constraints_qps.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	domain_qps.c -o domain_qps.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	polynomial_lib/polynomial.c -o polynomial_lib.o
	libtool --mode=link gcc -O isl_test.o constraints_qps.o domain_qps.o isl_utilities.o polynomial_lib.o ../libisl.la -lm -o isl_test

clean:
	rm -rf *.o *~ isl_test
