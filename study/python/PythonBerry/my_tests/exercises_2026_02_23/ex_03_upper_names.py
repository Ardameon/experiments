# Упражнение 3: Генератор имён
# -----------------------------
# Дан список имён: ["Alice", "Bob", "Charlie", "Diana"].
# Создай новый список, где каждое имя в верхнем регистре.
# Используй генератор списка и метод .upper()
# Подсказка: [выражение for элемент in список]

names = ["Alice", "Bob", "Charlie", "Diana"]

# upper_names = []
#
# for name in names:
#     upper_names.append(name.upper())

upper_names = [name.upper() for name in names]

print(upper_names)

