#!/usr/bin/python3

from e2c import enter_to_continue
import os
import sys
import datetime
from datetime import datetime as dt
import time
import html

odds = [1, 3, 5, 7, 9, 11, 13, 15, 17, 19,
		21, 23, 25, 27, 29, 31, 33, 35, 37,
		39, 41, 43, 45, 47, 49, 51, 53, 55, 57, 59]

print(os.getcwd())
enter_to_continue()

print(os.environ["PATH"])
enter_to_continue()

print(os.getenv("HOME"))
enter_to_continue()

print(datetime.date.today())
print(datetime.date.isoformat(datetime.date.today()))
print(datetime.date.today().day)
print(datetime.date.today().month)
print(datetime.date.today().year)
enter_to_continue()

print(time.strftime("%I:%M"))
print(time.strftime("%I:%M %p %A"))
enter_to_continue()

var = html.escape("This HTML fragment contains a <script>script</script> tag.")
print(var)

var = html.unescape("I &hearts; Python's &lt;standard library&gt;.")
print(var)
enter_to_continue()

right_this_second = dt.today().second

print("This second", right_this_second, end=" ")

if right_this_second in odds:
	print("seems a little odd")
else:
	print("seems not odd at all")
enter_to_continue()

week_day = time.strftime("%A");
print("Today is", week_day)

if week_day == "Saturday":
	print("Party!!!")
elif week_day == "SUnday":
	print("recover")
else:
	print("Work! work ... work...")
enter_to_continue()
















