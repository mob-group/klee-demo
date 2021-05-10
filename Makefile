all: test.bc test

test.bc: test.c
	${KLEE_ROOT}/llvm-install/bin/clang \
		-I${KLEE_ROOT}/klee-install/include \
		-emit-llvm -c -g -O0 -Xclang -disable-O0-optnone \
		$^ -o $@

test: test.c
	${KLEE_ROOT}/llvm-install/bin/clang \
		-I${KLEE_ROOT}/klee-install/include \
		-L${KLEE_ROOT}/klee-install/lib64 \
		-lkleeRuntest \
		$^ -o $@

clean:
	rm test test.bc
