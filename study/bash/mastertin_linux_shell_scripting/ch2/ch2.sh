#!/bin/bash

echo "This is noraml echo with new line at end"
echo -n "This is echo with suppressed new line"
echo
echo -e "This is echo with anabled esc-sequences and new line suppression \c"
echo

echo -n "Enter your text: "
read
echo "Your text is: $REPLY"
echo

read -p "Enter your text to variable: " text
echo "Variable value: $text"
echo

read -sp "Enter your silent text to variable: " text
echo -e "\nYour silent text: $text"

read -p "Press any key" -n1
echo

read -sp "Press any key (will not printed)" -n1
echo

read -n1 -sp "Press any key to invoke param_shift.sh script"
echo
echo
./param_shift.sh one two free four five
echo

read -n1 -sp "Press any key to invoke param_iterating.sh script"
echo
params="one two \"free four\""
echo
echo "Script parameters: $params"
echo
./param_iterating.sh one two "free four"
echo

read -n1 -sp "Press any key to invoke param_and_options.sh script"
echo
params="-c -b 44 -a -d -- papa mama baba"
echo
echo "Script parameters: $params"
echo
./param_and_options.sh $params
echo
