#!/bin/bash

# Color variables
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[0;33m'
blue='\033[0;34m'
magenta='\033[0;35m'
cyan='\033[0;36m'
# Clear the color after that
clear='\033[0m'

# Text format
bold=$(tput bold)
normal=$(tput sgr0)

usage()
{
    echo -e ""
    echo -e "Usage: build.sh [options]"
    echo -e ""
    echo -e "Options:"
    echo -e ""
    echo -e "   --cleanall\t\t Clean all build artifacts"
    echo -e "   --clean\t\t Run clean build"
    echo -e "   --tests\t\t Build and run tests"
    echo -e "   --no-tests\t\t Build without tests"
    echo -e ""
}

echo -e "Script arguments($#): $*"
echo -e ""
echo -e "${yellow}${bold}WARNING! This script is not working yet!${normal}${clear}"

usage
