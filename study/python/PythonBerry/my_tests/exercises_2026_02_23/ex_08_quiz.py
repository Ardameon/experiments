# Упражнение 8: Мини-викторина
# -----------------------------
# Создай список вопросов и список правильных ответов.
# Пройдись по вопросам, запрашивая ответ у пользователя (input()).
# Подсчитай количество правильных ответов.
# Подсказка: используй enumerate() или zip() для прохода по двум спискам

import random

questions = [
    "В каком году я родился",
    "Как звали следопыта из Властелина колец",
    "Где живут пингвины",
    "Как зовут мою собаку",
    "Когда у меня день рождения"
]

answers = [
    "1991",
    "Арагорн",
    "В антарктике",
    "Мышка",
    "13 мая"
]

def quiz(questions_answer_list):
    (i, (question, answer)) = random.choice(questions_answer_list)

    print(f"{i}. {question}?")
    user_answer = input("Введите ответ: ")

    if (answer.lower() == user_answer.lower()):
        print("Верно!")
    else:
        print("Неверно! Правильный ответ -", answer)

# enumerate returns iterator, so it will "disappear" after iteration finished
quiz_enum = enumerate(zip(questions, answers), 1)

# for i, (question, answer) in quiz_enum:
#     print(f"{i}) {question} - {answer}")

quiz_list = list(enumerate(zip(questions, answers), 1))

quiz(quiz_list)


