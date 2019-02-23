#!/usr/bin/python3

from e2c import enter_to_continue
import os
import sys
import datetime
import time
import html

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