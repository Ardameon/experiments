#!/bin/bash

echo "Chapter 3: Conditions"
echo

test 1 -lt 2 && echo "1 is less then 2"
[ 1 -lt 2 ] && echo "1 is less then 2"

if [ -n $SSH_TTY ]; then
    echo "This is SSH terminal connection"
else
    echo "This is not SSH terminal connection"
fi

if ! [ $# -eq 0 ] && [ ! $# -eq 0 ]
then
    echo "This script is invoked with parameters - $*"
else
    echo "This script is inovked without paramters"
fi

echo
echo "IF statement script tests"
echo
./if_statement.sh
./if_statement.sh 1
./if_statement.sh 1 2
./if_statement.sh 1 2 3
echo
echo "CASE statement script tests"
echo
./case_statement.sh AA C bb r c 234 -1 19993 20001 name1 name2 name3 name

