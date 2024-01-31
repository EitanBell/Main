target: prerequisites

.PHONY: do run

do: 
	gcc -g -c static_func.c
	ar rcs lib_stat.a  static_func.o
	gcc -g -c -fPIC dynamic_exp_func.c
	gcc -g -shared dynamic_exp_func.o -o lib_dynamic_exp.so
	gcc -g -c -fPIC dynamic_imp_func.c
	gcc -g -shared dynamic_imp_func.o -o lib_dynamic_imp.so
	gcc -g mem_map_full.c -L. -ldl -l_stat -l_dynamic_imp -Wl,-rpath,. -o mem_map_full

run:
	./mem_map_full

prerequisites: do

target: prerequisites
