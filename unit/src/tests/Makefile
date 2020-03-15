OBJ_DIR = ../build/obj
INCLUDE = -I../include
CFLAGS += $(INCLUDE)


all:
	g++ $(CFLAGS) -o tests tests_main.cpp test_increase.cpp increase_moc.cpp $(OBJ_DIR)/increase.o -lCppUTest -lCppUTestExt

test:
	g++ -o tests tests_main.cpp test.cpp -lCppUTest -lCppUTestExt

.PHONY: all test
