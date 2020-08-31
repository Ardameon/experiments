CXX = g++

WARNINGS = -Wall -Wextra -Werror -Wno-error=cpp
OPTIMIZATION = -O0
STD = -std=c++11
DBG = -g

CFLAGS += $(STD) $(WARNINGS) $(OPTIMIZATION) $(DBG)

C10_DIR = ./c10_classes_base

all: c10


c10:
	$(MAKE) -C $(C10_DIR) CFLAGS="$(CFLAGS)"





.PHONY: all c10
