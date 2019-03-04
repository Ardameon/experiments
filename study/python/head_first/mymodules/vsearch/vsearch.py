#!/usr/bin/python3

def search4vovels(word:str) -> set:
  """Search vovels in word parameter"""
  vovels = set("ueoia")
  return vovels.intersection(set(word)) 

def search4letters(phrase:str, letters:str = 'aeiou') -> set:
  """Search letters in phrase"""
  return set(letters).intersection(set(phrase)) 


