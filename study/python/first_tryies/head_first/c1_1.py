#!/usr/bin/python3

from e2c import enter_to_continue
from datetime import datetime as dt
import time
import random

odds = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19,
		21, 23, 25, 27, 29, 31, 33, 35, 37,
		39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59]



mylist = []

for ch in "Hello!":
	mylist += ch

print(mylist)
enter_to_continue()


for i in range(5):
	right_this_second = dt.today().second
	print("This second", right_this_second, end=" ")
	if right_this_second in odds:
		print("seems a little odd")
	else:
		print("seems not odd at all")

	wait_time = random.randint(1, 4) 
	time.sleep(wait_time)



enter_to_continue()

















