# Divider and Multiplier Formal Verifier

## btor2Translator

* step 1: unroll the cycles to reduce registers in original btor2 file;

* step 2: translate the unrolling btor2 file to polynomials.

## build

CMake will automatic download [btor2tools](https://github.com/boolector/btor2tools) as dependency.

```shell
mkdir build && cd build
cmake ..
make
```

You will find an executable binary file ***btorTrans***.

## test

Run ***btorTrans*** to get polynomials.

```shell
./btorTrans examples/DividerFixedClock_2bit.btor2 
```
