#!/bin/python3

# Convert "Don't panic!" to "on tap"

phrase = "Don't panic!"
plist = list(phrase)

print(phrase)
print(plist)

# My method
new_plist = []
new_plist.extend(plist[1:3])
new_plist.extend(plist[5:3:-1])
new_plist.extend(plist[-5:-7:-1])

new_phrase = "".join(new_plist)

print(new_phrase)
print(new_plist)

# Book method
new_phrase = "".join(plist[1:3])
new_phrase = new_phrase + "".join([plist[5], plist[4], plist[7], plist[6]])

print(new_phrase)
print(plist)
