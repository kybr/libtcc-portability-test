#!/bin/sh
rm -rf tcc
cd tinycc
make distclean
./configure --prefix=../tcc
make -j 10
make install
