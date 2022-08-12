# Divider and Multipiler Formal Verifier

## btor2Translator

* step 1: unroll the cycles to reduce registers in original btor2 file;

* step 2: translate the unrolling btor2 file to polynomials. 

## build
```
mkdir deps
cd deps
git clone https://github.com/Boolector/btor2tools.git
```

Enter deps/btor2tools directory, and build [btor2tools](https://github.com/boolector/btor2tools) as follows:
```
./configure.sh
cd build
make
```

Back the Div&MulFormalVerifier root directory and build as follows:
```
mkdir build && cd build
cmake ..
make
```

Then you will find an executable binary file ***btorTrans***.

## test

Run ***btorTrans*** to get polynomials.
```
./btorTrans examples/DividerFixedClock_2bit.btor2 
```

