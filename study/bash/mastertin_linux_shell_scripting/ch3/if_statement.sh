#!/bin/bash

echo "This script is - $0"
echo "Script parameters: $@"

if [ $# -eq 0 ]; then
    if test -f $0; then
        echo "$0 is regular file"
        echo "$(basename $0) is $(file $0)"
    else
        echo "$0 is not regular file"
        echo "$(basename $0) is $(file $0)"
    fi

    if [ -O $0 ]; then
        echo "$USER is owner of this script"
    else
        echo "$USER is not owner of this script"
    fi

    [ -r $0 -o -x $0 ] && echo "This file is readable or executable"
    [ -r $0 -a -x $0 ] && echo "This file is readable and executable"
    [ -e $0 ] && echo "This file exists"
    [ -b $0 ] || echo "This file is not block device"
    [ -c $0 ] || echo "This file is not char device"
    [ -p $0 ] || echo "This file is not pipe"
    [ -d $0 ] || echo "This file is not a directory"
    [ $0 -nt .. ] && echo "This file newer then .."
    [ $0 -ot .. ] && echo "This file older then .."
fi

if [ $# -gt 2 ]; then
    echo "This script has more then 2 parameter"
elif [ $# -eq 2 ]; then
    echo "This script has 2 parameters"
else
    echo "This script has less then 2 parameters"
fi
