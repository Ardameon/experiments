#!/usr/bin/python3

import os
import io
import sys
from sys import argv

pwd = os.getcwd()

if len(argv) > 1:
    filename = argv[1] if argv[1][-2:] == '.c' else argv[1] + '.c'
else:
    filename = 'program.c'

try:
    f = io.open(filename, 'x')
except FileExistsError:
    f = io.open(filename, 'w')

c_template = '''\
#include <stdio.h>

int main(int argc, char **argv)
{
    printf("hello world\\n");
    return 0;
}


'''

f.write(c_template)

f.close()

