#!/usr/bin/python3

def revert_string(string):
    """Return reverted string"""
    l = list(string)
    

    return "".join(l[::-1])

print(revert_string("Option"))
print(revert_string("Base"))
print(revert_string("tennet"))
print(revert_string("Hello world"))
