CXX = g++

WARNINGS = -Wall -Wextra -Werror -Wno-error=cpp
OPTIMIZATION = -O0
STD = -std=c++11
DBG = -g

CFLAGS += $(STD) $(WARNINGS) $(OPTIMIZATION) $(DBG)

C10_DIR = ./c10_classes_base
C11_DIR = ./c11_overloads_friends

all: c10 c11


c10:
	$(MAKE) -C $(C10_DIR) CFLAGS="$(CFLAGS)" all

c11:
	$(MAKE) -C $(C11_DIR) CFLAGS="$(CFLAGS)" all





.PHONY: all c10 c11
