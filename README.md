# Building

## Setup

The build scripts put everything in `$KLEE_ROOT` for simplicity. You will need
to install Z3 using the system package manager.

```shell
export KLEE_ROOT=path/to/some/root/dir
```

## LLVM 9

Might work on newer versions of LLVM; unsure what upstream KLEE supports
properly so sticking with 9 for now.

```shell
cd $KLEE_ROOT
git clone git@github.com:llvm/llvm-project.git
cd llvm-project/
git checkout llvmorg-9.0.1
mkdir build
cd build
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$KLEE_ROOT/llvm-install \
  -DLLVM_TARGETS_TO_BUILD=X86 \
  -DLLVM_ENABLE_PROJECTS="clang;libcxx;libcxxabi" \
  ../llvm
make -j`nproc`
make install
```

## KLEE-Float

```shell
cd $KLEE_ROOT
git clone git@github.com:mob-group/klee.git
cd klee
mkdir build
cd build
cmake \
  -DENABLE_SOLVER_Z3=On \
  -DENABLE_FLOATING_POINT=On \
  -DENABLE_FP_RUNTIME=On \
  -DLLVM_CONFIG_BINARY=$KLEE_ROOT/llvm-install/bin/llvm-config \
  -DCMAKE_INSTALL_PREFIX=$KLEE_ROOT/klee-install \
  -DENABLE_UNIT_TESTS=OFF \
  -DENABLE_SYSTEM_TESTS=Off \
  ../
make -j`nproc`
make install
```

## Demo

```shell
cd $KLEE_ROOT
git clone git@github.com:mob-group/klee-demo.git
cd klee-demo
```
