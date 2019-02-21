#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import sys
import e2c

e2c.enter_to_continue()

print(type(False))
print(type(1))
print(type(1.2))
print(type(2 + 2j))
print(type('Строка'))
print(type('String'))
print(type(bytes("Строка", "UTF-8")))
print(type(bytearray("Строка", "UTF-8")))
print(type([1, 2, 3]))
print(type((1, 2, 3)))
print(type(range(1, 2)))
print(type({"a": 1, "b": 2}))
print(type({"a", "b"}))
print(type(e2c.enter_to_continue))
print(type(sys))

e2c.enter_to_continue()

arr = [1, 2 ,3]
print(arr)
arr[0] = 0
print(arr)

e2c.enter_to_continue()

str1 = "auto"
str2 = "transport"
str3 = str1 + str2
print(str3)

e2c.enter_to_continue()

i = iter(arr)
print(type(i))
print(i)
print(i.__next__())
print(next(i))

e2c.enter_to_continue()

for i in arr:
	print(i)

e2c.enter_to_continue()

for i in 'string':
	print(i + '-', end="")
print()

e2c.enter_to_continue()

d = {"a": 0, "b": 1, "c": 2}
for i in d:
	print("%s: " % i, d[i])

e2c.enter_to_continue()





