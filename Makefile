HEADER_DIR=-I../include/ -I. -Iisl/non-linear-FM/ -Ipolynomial_lib/
LIB_DIR=-L../


all:
	gcc -c ${HEADER_DIR} ${LIB_DIR} nfm_test.c -o nfm_test.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} nfm_isl_interface.c -o nfm_isl_interface.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} nfm_constraint.c -o nfm_constraint.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	nfm_domain.c -o nfm_domain.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	nfm_union_domain.c -o nfm_union_domain.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	nfm_polynome.c -o nfm_polynome.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	nfm_space.c -o nfm_space.o
	libtool --mode=link gcc -O nfm_test.o nfm_constraint.o nfm_union_domain.o nfm_domain.o nfm_isl_interface.o polynomial_lib.o ../libisl.la -lm  nfm_polynome.o nfm_space.o -o nfm_test

clean:
	rm -rf *.o *~ nfm_test .libs
