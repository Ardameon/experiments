#!/usr/bin/python3

from e2c import enter_to_continue

# sets

vovels = set("uuueeoiiaaa")
word = "hello"

print(vovels)
print(set(word))

u = vovels.union(set(word))
d = vovels.difference(set(word))
i = vovels.intersection(set(word))

print("union", u)
print("difference", d)
print("intersection", i)


word = input("Provide the word to search for vovels: ")
print(list(vovels.intersection(set(word))))

enter_to_continue()

# tuples

vovels = ['a', 'e', 'i', 'o', 'u']
vovels2 = ('a', 'e', 'i', 'o', 'u')

print(vovels)
print(vovels2)

enter_to_continue()

t1 = ('tuple')
print(t1, type(t1))
t2 = ('tuple',)
print(t2, type(t2))

enter_to_continue()

import pprint

people = {}

people['Ford'] = {'Name': 'Ford Prefect',
                  'Gender': 'Male',
                  'Occupation': 'Researcher',
                  'Home Planet': 'Batelgeuse Seven'}

people['Arthur'] = {'Name': 'Arthur Dent',
                    'Gender': 'Male',
                    'Occupation': 'Sandwich-maker',
                    'Home Planet': 'earth'}

people['Thrillian'] = {'Name': 'Tricia McMillan',
                  'Gender': 'Female',
                  'Occupation': 'Mathematician',
                  'Home Planet': 'Earth'}

people['Robot'] = {'Name': 'Marvin',
                  'Gender': 'Unknown',
                  'Occupation': 'Paranoid Android',
                  'Home Planet': 'Unknown'}

print(people)

enter_to_continue()

pprint.pprint(people)

enter_to_continue()

print("Arthur's occupation is -", people['Arthur']['Occupation'])







