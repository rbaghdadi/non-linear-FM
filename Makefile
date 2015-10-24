HEADER_DIR=-I../include/ -I. -Iisl/non-linear-FM/ -Ipolynomial_lib/
LIB_DIR=-L../


all:
	gcc -c ${HEADER_DIR} ${LIB_DIR} isl_test.c -o isl_test.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} nfm_isl_interface.c -o nfm_isl_interface.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} nfm_constraint.c -o nfm_constraint.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	nfm_domain.c -o nfm_domain.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	polynomial_lib/polynomial.c -o polynomial_lib.o
	libtool --mode=link gcc -O isl_test.o nfm_constraint.o nfm_domain.o nfm_isl_interface.o polynomial_lib.o ../libisl.la -lm -o isl_test

clean:
	rm -rf *.o *~ isl_test
