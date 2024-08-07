#!/bin/bash

# Load text formating variables
source ./text_fmt

log_success()
{
    echo
    echo -e "${green}${bold}$1${normal}${clear}"
    echo
}

log_warn()
{
    echo
    echo -e "${yellow}${bold}$1${normal}${clear}"
    echo
}

log_err()
{
    echo
    echo -e "${red}${bold}$1${normal}${clear}"
    echo
}

CLEANALL=false
CLEAN=false
TESTS=false
NO_TESTS=false

BUILD_ROOT=".."
BUILD_DIRNAME="build"
BUILD_DIR="$BUILD_ROOT/$BUILD_DIRNAME"

APP_DIR="$BUILD_DIR/array"
TESTS_DIR="$APP_DIR/tests"
APP_NAME="array"
TESTS_NAME="array_tests"

APP_BIN="$APP_DIR/$APP_NAME"
TESTS_BIN="$TESTS_DIR/$TESTS_NAME"

usage()
{
    echo -e ""
    echo -e "Usage: build.sh [options]"
    echo -e ""
    echo -e "Options:"
    echo -e ""
    echo -e "   --cleanall\t\t Clean all build artifacts"
    echo -e "   --clean\t\t Run clean build"
    echo -e "   --tests\t\t Build and run tests only"
    echo -e "   --no-tests\t\t Build without tests"
    echo -e "   --help\t\t Show this message"
    echo -e ""
}

check_options()
{
    exit 0
}

exit_error()
{
    usage
    exit 1
}

cmd_exec_check()
{
    [ $# -lt 3 ] && log_err "Command execution check: not enough argumets - less then 3" && exit 200

    local exec_res=$1
    local exec_str=$2
    local exit_code=$3

    if [ $exec_res -eq 0 ]; then
        log_success "$exec_str: success"
    else
        log_err "$exec_str: failed"
        exit $exit_code
    fi
}

make_build_dir()
{
    [ -d $BUILD_DIR ] && echo "Build dir already exists: $BUILD_DIR" && return

    echo -e "Make build dir: $BUILD_DIR"

    pushd $BUILD_ROOT || return
    cmake -B $BUILD_DIRNAME
    cmd_exec_check $? "Make build dir" 1
    popd || return
}

build_app()
{
    [ ! -d $BUILD_DIR ] && make_build_dir

    echo -e "Build app: $APP_BIN"

    pushd $BUILD_DIR || return
    make $APP_NAME
    cmd_exec_check $? "Build app" 2
    popd || return
}


build_tests()
{
    [ ! -d $BUILD_DIR ] && make_build_dir

    echo -e "Build tests: $TESTS_BIN"

    pushd $BUILD_DIR || return
    make $TESTS_NAME
    cmd_exec_check $? "Build tests" 3
    popd || return
}

run_tests()
{
    ./$TESTS_BIN -v -c
    cmd_exec_check $? "Run tests" 4
}

clean()
{
    if [ -d $BUILD_DIR ]; then
        echo -e "Clean build"
        pushd $BUILD_DIR || return
        make clean
        popd || return
        log_success "Clean success"
    else
        log_err "Build directory \"$BUILD_DIR\" doesn't exist"
        exit 6
    fi
}

cleanall()
{
    if [ -d $BUILD_DIR ]; then
        echo -e "Clean all build directory"
        rm -rf $BUILD_DIR
        log_success "Cleanall success"
    else
        log_err "Build directory \"$BUILD_DIR\" doesn't exist"
        exit 7
    fi
}

echo -e "Script arguments($#): $*"
log_warn "WARNING! This script is not working correctly yet!"

#build_app
[ -x $TESTS_BIN ] || build_tests
run_tests
clean
#cleanall

