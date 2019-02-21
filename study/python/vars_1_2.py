#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import sys
import e2c

a = 4
# b = "string"
b = 1.1

if type(a) == int:
	print(a, " - int")

if type(b) == str:
	print(b, " - str")
else:
	print(b, " - not str")

if isinstance(b, float):
	print(b, " - float")
else:
	print(b, " - not float")

e2c.enter_to_continue()

print(bool(1.1), bool(0), int(7.5), int("72"), int("0xBF", 16), str(7.6666))

e2c.enter_to_continue()

print(bytes("строка", "utf-8"))
print(bytes("строка", "cp1251"))
print(bytearray("строка", "cp1251"))

e2c.enter_to_continue()

x = input("x = ")
y = input("y = ")
print("x + y = ", x + y)

e2c.enter_to_continue()

x = int(input("x = "))
y = int(input("y = "))
print("x + y = ", x + y)





