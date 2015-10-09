HEADER_DIR=-I../include/ -I.
LIB_DIR=-L../


all:
	gcc -c ${HEADER_DIR} ${LIB_DIR} isl_test.c -o isl_test.o
	gcc -c ${HEADER_DIR} ${LIB_DIR} constraints_qps.c -o constraints_qps.o
	gcc -c ${HEADER_DIR} ${LIB_DIR}	domain_qps.c -o domain_qps.o
	libtool --mode=link gcc -O isl_test.o constraints_qps.o domain_qps.o ../libisl.la -o isl_test

clean:
	rm -rf *.o *~ isl_test
