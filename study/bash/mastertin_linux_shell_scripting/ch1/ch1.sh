#!/bin/bash

#alias separate='echo "";echo "------------------";echo ""'

echo "";echo "------------------";echo ""
echo "Chapter 1: The What and Why of Scripting with Bash"
echo "";echo "------------------";echo ""

# Show type of command
type ls
type if
type man
# Show all types of command
type -a ls
# Show the only type of command
type -t ls
type -t if

echo "";echo "------------------";echo ""
# Execute another script from script
./hello1.sh
echo "exit status $?"

echo "";echo "------------------";echo ""
# Execute second cmd only if first will fail
ls || echo "this will not be displayed"
type ujjddjj || echo "prev command is failed"

echo "";echo "------------------";echo ""
# Execute second cmd only if first will success
type ujjddjj && echo "this will not be displayed"
ls && echo "prev command is success"

echo "";echo "------------------";echo ""
# Execute scritpt with arguments handling
./arguments.sh one two three four five six seven eight nine ten eleven

echo "";echo "------------------";echo ""
# Masking $ in strings '' and ""
echo "$USER owns $4"
echo '$USER owns $4'
echo "$USER owns \$4"

echo "";echo "------------------";echo ""
# Exeution command in command
echo "Your location is $(pwd)"
echo "Your location is `pwd`"

echo "";echo "------------------";echo ""
# Take basename from path
echo `pwd`
basename $(pwd)
basename $(echo "$HOME")

echo "";echo "------------------";echo ""
# Variables (should no spaces around '=')
my_var=
name="Petya"
age=35
name_age="$my_var $name $age"
echo "$name_age"

echo "";echo "------------------";echo ""
# Arrays
my_arr=(one two three four)
echo $my_arr
echo "$my_arr[*]"
echo ${my_arr[*]}
echo ${my_arr[1]}

echo "";echo "------------------";echo ""
# Unset removes element from array
unset my_arr[1]
echo "Array without 1st element: ${my_arr[*]}"

echo "";echo "------------------";echo ""
# Envirenment variables
printenv
echo "$USER"
printenv HOME
echo "$HOME"
echo "$BASH_VERSION"

echo "";echo "------------------";echo ""
# Exporting variables
TEST_VAR="Out value"
echo "Out value - $TEST_VAR"
./env_var.sh
echo "Out value after script - $TEST_VAR"

echo "";echo "------------------";echo ""
# Eported variables copied to invoked commands
export TEST_VAR
echo "Out value - $TEST_VAR"
./env_var.sh
# Inner changes of exported variables will not affect out script
echo "Out value after script - $TEST_VAR"

echo "";echo "------------------";echo ""
# Import variables from other file
echo "Out value - $VAR"
source vars
# Now $VAR value was imported
echo "Out value after import - $VAR"

echo "";echo "------------------";echo ""
# CMD substitution
current_path=$(pwd)
echo $current_path

echo "";echo "------------------";echo ""
# scritps DEBUG more info
bash -v hello1.sh
echo "";echo "------------------";echo ""
# scritps DEBUG execution info
bash -x hello1.sh

#PATH=$PATH:.
#echo "$PATH"
