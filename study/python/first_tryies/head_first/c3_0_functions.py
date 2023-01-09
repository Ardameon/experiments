#!/usr/bin/python3

from e2c import enter_to_continue

# sets

def search4vovels_0(word):
  """Search vovels in inpt from stdin"""
  vovels = set("ueoia")
  print(list(vovels.intersection(set(word))))

def search4vovels_1(word):
  """Search vovels in word parameter"""
  vovels = set("ueoia")
  result = list(vovels.intersection(set(word))) 
  print(result)
  # return True if len(result) else False
  return bool(result)

def search4vovels_2(word:str) -> set:
  """Search vovels in word parameter"""
  vovels = set("ueoia")
  return vovels.intersection(set(word)) 

def search4letters(phrase:str, letters:str = 'aeiou') -> set:
  """Search letters in phrase"""
  return set(letters).intersection(set(phrase)) 



# word = input("Provide the word to search for vovels: ")

search4vovels_0(input("Provide the word to search for vovels: "))
res = search4vovels_1(input("Provide the word to search for vovels: "))
print(res)
res = search4vovels_2(input("Provide the word to search for vovels: "))
print(res)

# enter_to_continue()

# help(search4vovels_2)

enter_to_continue()

p = input("Provide the word to search for letters: ")
l = input("letters to seaech: ")

res = search4letters(p, l)
print(res)

res = search4letters(p)
print(res)

res = search4letters(letters = l, phrase = p)
print(res)

enter_to_continue()








