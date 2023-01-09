#!/usr/bin/python3

from e2c import enter_to_continue

person3 = {'Name': 'Ford Prefect',
		   'Gender': 'Male',
		   'Occupation': 'Researcher',
		   'Home Planet': 'Batelgeuse Seven'}

person3['Age'] = 33

print(person3)

enter_to_continue()

vovels = ['u', 'i', 'o', 'e', 'a']
word = input("Provide the word to search for vovels: ")

found = {}

for char in vovels:
	found[char] = 0

print(found)

for letter in word:
	if letter in vovels:
		found[letter] += 1

for key in found:
	print(key, "is found", found[key], "times")

enter_to_continue()

for key in sorted(found):
	print(key, "is found", found[key], "times")

enter_to_continue()

#items
for key, val in sorted(found.items()):
	print(key, "is found", val, "times")

enter_to_continue()

found_ninit = {}

for letter in word:
	if letter in vovels:
		# if letter in found_ninit:
			# found_ninit[letter] += 1
		# else:
			# found_ninit[letter] = 1

		# if letter not in found_ninit:
			# found_ninit[letter] = 0
		# found_ninit[letter] += 1
		
		# found_ninit[letter] = found_ninit[letter] + 1 if letter in found_ninit else 1

		found_ninit.setdefault(letter, 0)
		found_ninit[letter] += 1



for key, val in sorted(found_ninit.items()):
	print(key, "is found", val, "times")

enter_to_continue()










