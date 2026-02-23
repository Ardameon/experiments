# Упражнение 4: Палиндром
# ------------------------
# Напиши код, который проверяет, является ли строка палиндромом.
# Палиндром — строка, которая читается одинаково в обе стороны.
# Подсказка: используй срез [::-1] для разворота строки.

# def is_palindrome(str):
#     revert_str = str[::-1]
#
#     for i in range(len(str)):
#         if str[i] != revert_str[i]:
#             return False
#
#     return True

def is_palindrome(s):
    return s == s[::-1]

s = input("Введите строку для проверки на палиндром: ")

if is_palindrome(s):
    print(s, " - палиндром")
else:
    print(s, " - не палиндром")

