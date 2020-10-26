CXX = g++

WARNINGS = -Wall -Wextra -Werror -Wno-error=cpp
OPTIMIZATION = -O0
STD = -std=c++11
DBG = -g

CFLAGS += $(STD) $(WARNINGS) $(OPTIMIZATION) $(DBG)

A01_DIR = ./a01_SetClass
A02_DIR = ./a02_LinkedList

all: a01 a02

a01:
	$(MAKE) -C $(A01_DIR) CFLAGS="$(CFLAGS)"
	
a02:
	$(MAKE) -C $(A02_DIR) CFLAGS="$(CFLAGS)"





.PHONY: all c10
