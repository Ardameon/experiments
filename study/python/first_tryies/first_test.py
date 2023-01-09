#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import sys

def enter_to_continue():
	print("Press \"Enter\" to continue...")
	input()

def write_hello():
	print ("Hello world:Привет!")
	print('argv =', sys.argv, '(%s)' % len(sys.argv))
	print('x = %s' % x)
	print('x =', x, '- this is the result')
	print(arr)
	print(arr_s)


x = 30 + 10 \
	+ 5

arr = [1, 2, 3, 4]
arr_s = {'key1': 10, 'key2': 20, 'key3': 30}

write_hello()
enter_to_continue()

print("""Warning!
	This text is
	                  formated
	        manually
	        """)

enter_to_continue()

sys.stdout.write("Dirrect write by sys.stdout\n")

enter_to_continue()

name = input("Enter your name: ")
print("Hello,", name)

enter_to_continue()

equation = input("Enter your equation (e.g. 1 + 2): ")
result = eval(equation)
print(equation, "=", result)

enter_to_continue()

list = sys.argv[:]
for n in list:
	print(n)

enter_to_continue()























