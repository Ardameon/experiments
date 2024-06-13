#!/bin/bash

echo "$@"
echo "$*"
echo "$#"
echo "Script arguments: $*"
shift
echo "Script arguments after shift: $*"
