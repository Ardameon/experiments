#!/bin/python3

vovels = ['a', 'e', 'i', 'o', 'u']
word = input("Provide a word for search for vovels: ")
found = []

for letter in word:
    if letter in vovels:
        if letter not in found:
            found.append(letter)

for vovel in found:
    print(vovel)
