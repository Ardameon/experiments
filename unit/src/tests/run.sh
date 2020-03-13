#!/bin/bash

make -C .. all
make
./tests -c -v
