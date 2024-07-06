#!/bin/bash

echo "This script is - $(basename $0)"
echo "Script parameters: $@"

while [ -n "$1" ]; do
    case "$1" in
        [A-C])
            echo "Parameter $1 is [A-C]";;
        [abr])
            echo "Parameter $1 is [abr]";;
        [1-20000])
            echo "Parameter $1 is [1-20000]";;
        "name[1-3]")
            echo "Parameter $1 is name[1-3]";;
        "name1")
            echo "Parameter $1 is name1";;
        name2)
            echo "Parameter $1 is name2";;
        *)
            echo "Unknown parametr $1";;
    esac
    shift
done
