all: test.bc test

test.bc: test.c
	${KLEE_ROOT}/llvm-install/bin/clang \
		-I${KLEE_ROOT}/klee-install/include \
		-emit-llvm -c -g -O0 -Xclang -disable-O0-optnone \
		$^ -o $@

test: test.c
	${KLEE_ROOT}/llvm-install/bin/clang \
		-DKLEE_RUNTIME \
		-I${KLEE_ROOT}/klee-install/include \
		-L${KLEE_ROOT}/klee-install/lib64 \
		-lkleeRuntest -lm \
		$^ -o $@

verify: test.bc all
	${KLEE_ROOT}/klee-install/bin/klee $<

.PHONY: clean
clean:
	rm test test.bc

.PHONY: clean-all
clean-all: clean
	rm -rf klee-*
