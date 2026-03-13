# Упражнение 9: Генератор паролей (бонус)
# ----------------------------------------
# Напиши программу, которая:
# 1. Создаёт список символов (буквы, цифры, спецсимволы)
# 2. Случайным образом выбирает 8–12 символов
# 3. Собирает их в строку через join()
# 4. Выводит готовый пароль
# Подсказка: используй random.choice() в цикле или random.sample()

import random
import string

symbols = string.ascii_letters + string.digits + string.punctuation

# password = "".join(random.sample(symbols, 12))

password = ""

for i in range(12):
    password += random.choice(symbols)

print(password)
