OBJ_DIR = ../build/obj
INCLUDE = -I../include
CFLAGS += $(INCLUDE)
COV = -fprofile-arcs -ftest-coverage -fPIC -O0

CPPUTEST_USE_GCOV=Y

all:
	g++ $(CFLAGS) -o tests tests_main.cpp test_increase.cpp increase_moc.cpp $(OBJ_DIR)/increase.o -lCppUTest -lCppUTestExt

cov:
	g++ $(CFLAGS) $(COV) -o tests tests_main.cpp test_increase.cpp increase_moc.cpp $(OBJ_DIR)/increase.o -lCppUTest -lCppUTestExt

test:
	g++ -o tests tests_main.cpp test.cpp -lCppUTest -lCppUTestExt
	
clean:
	rm -f *.gcda
	rm -f *.gcno

.PHONY: all test cov clean
