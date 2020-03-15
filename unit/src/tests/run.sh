#!/bin/bash

PARAM=$1

case "$PARAM" in
    "cov" | "coverage" )
        make -C .. clean
        CFLAGS="-fprofile-arcs -ftest-coverage -fPIC -O0" make -C .. all
        make clean
        make cov
        ./tests -c -v
        gcovr -r ..
        ;;
    * )
        make -C .. clean
        make -C .. all
        make clean
        make
        ./tests -c -v
        ;;
esac


