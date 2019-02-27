#!/usr/bin/python3

from e2c import enter_to_continue

vovels = ['a', 'e', 'i', 'o', 'u']
word = input("Provide the word to search for vovers: ")

for letter in word:
	if letter in vovels:
		print(letter)

enter_to_continue()

found = []

for letter in word:
	if letter in vovels:
		if letter not in found:
			found.append(letter)

print(found)

enter_to_continue()

phrase = "Don't panic"
plist = list(phrase)

print(phrase)
print(plist)

plist.remove('D')
plist.remove('\'')
for i in range(3):
	plist.pop()
plist.insert(2, plist.pop(3))
plist.extend([plist.pop(), plist.pop()])

for letter in plist:
	print(letter, end = '')
print()
print(str(plist))

new_phrase = "".join(plist)
print(new_phrase)

enter_to_continue()

plist = list(phrase)

print(plist[3:])
print(plist[:5])
print(plist[::])
print(plist[2:9:3]) # [start:stop:step]

enter_to_continue()

# Don't panic -> on tap
new_phrase = "".join(plist[1:3] + plist[5:3:-1] + plist[-4:-6:-1])
print(plist)
print(new_phrase)

enter_to_continue()

paranoid_android = "Marvin, the paranoid android"
letters = list(paranoid_android)

for char in letters[:6]:
	print('\t', char)
print()

for char in letters[-7:]:
	print('\t' * 2, char)
print()

for char in letters[12:20]:
	print('\t' * 3, char)
print()

enter_to_continue()





