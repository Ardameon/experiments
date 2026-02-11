#!/bin/python3

# Convert "Don't panic!" to "on tap"

phrase = "Don't panic!"
plist = list(phrase)

print(phrase)
print(plist)

plist.pop(0);
plist.remove("'");

for i in range(4):
    plist.pop()

# "ont pa"

plist.extend([plist.pop(), plist.pop()])

# "ont ap"

plist.insert(2, plist.pop(3))

# "on tap"

phrase = "".join(plist)

print(phrase)
print(plist)
