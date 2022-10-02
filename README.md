# Matrix
Let's writa a class of a matrix m * b, where m and n are comparably small numbers:
we need to support:
 - move-constructor;
 - copy-costructor;
 - iteration in range-based loops;
 - concatanation with operator |;
 - indexing with the help [];
 - sum.

# Building

```
git clone -b master --recursive git@github.com:SafinaM/matrix.git 
cd matrix
mkdir build
cd build
cmake ../src
cmake --build . --config Release -- -j 4
./matrix
```
