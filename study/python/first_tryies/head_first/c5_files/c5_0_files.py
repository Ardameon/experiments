#!/usr/bin/python3

from e2c import enter_to_continue

fname_todos = 'todos.txt'

print('Print to file', fname_todos)

todos = open(fname_todos, 'a')

print('Put out the trash', file = todos)
print('Feed thecat', file = todos)
print('Prepare tax return', file = todos)

todos.close()

enter_to_continue()

tasks = open(fname_todos)

for chore in tasks:
    print(chore, end = '')

tasks.close()

enter_to_continue()

#with variant - no need to call close() at end
with open(fname_todos) as tasks:
    for chore in tasks:
        print(chore, end = '')
     






