#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import sys
import e2c

x = y = [1, 2]
print(x, y)
x[0] = 100
print(x, y)
print("x is y = ", x is y)
print(sys.getrefcount(x))

e2c.enter_to_continue()

x = [1, 2]
y = [1, 2]
print(x, y)
x[0] = 100
print(x, y)
print("x is y = ", x is y)
print(sys.getrefcount(x))

e2c.enter_to_continue()

x, y, z = 3, 2, 1
print(x, y, z)

e2c.enter_to_continue()

x, y = y, x
print(x, y)

e2c.enter_to_continue()

arr = [3, 2, 1]
x, y, z = arr
print(x, y, z)
arr[0] = 300
arr[1] = 400
print(x, y, z)

e2c.enter_to_continue()

[a, b, c] = (10, 20, "string")
print(a, b, c)

e2c.enter_to_continue()

a, *b, c = [1, 2, 3, 4]
print(a, b, c)

e2c.enter_to_continue()

a, *b = 1, 2, 3, 4, 5, 6
print(a, b)

e2c.enter_to_continue()












