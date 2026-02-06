`Python` - интерпретируемый язык 

В python нет понятия функции `main()`. 
Код не компилируется, а сразу выполняется интерпретатором. 

[Ссылка на документацию языка Python](https://docs.python.org/3/)

1. [[#Подключение модулей]]
2. [[#Списки]]
3. [[#Оператор if]]
4. [[#Цикл for]]
5. [[#Получение справки]]
	1. [[#dir()]]
	2. [[#help()]]
6. [[#Функция range()]]
7. [[#Дополнительный код]]

---
### Подключение модулей

В стандартной библиотеке Python есть множество уже готовых модулей, которые идут из коробки.

Подключение модуля:

```python
import datetime
import os

datetime.datetime.today()
os.getcwd()
```

Подключение отдельного субмодуля субмодуля:

```python
from datetime import datetime
from os import getcwd

datetime.today()
getcwd()
```

Примеры стандартных модулей:
* os
* time
* datetime
* sys
* random
* enum
* json
* html

Примеры:

```python
import sys
import os
import time

sys.platform
# 'linux'
sys.version
# '3.10.12 (main, Jan  8 2026, 06:52:19) [GCC 11.4.0]'
os.environ
# environ({'SHELL': '/bin/bash', 'SESSION_MANAGER': 'local/pylesosik-noobook:@/tmp/.ICE-unix/2944,unix/pylesosik-noobook:/tmp/.ICE-unix/2944', 'QT_ACCESSIBILITY': '1', 'COLORTERM': 'truecolor', ...
os.getenv('HOME')
# '/home/pylesosik'
time.strftime("%A %I:%M %p")
# 'Friday 07:15 PM'
```

---
### Списки

Список стандартная структура данных языка обозначается как `[]`
Списки могут хранить ==любые== данные любых типов, и даже смешанные.

Примеры списков:

```Python
[ 2, 3, 4, 5]
[ 'one', 'two', 'three' ]
[ 1, 2, 'three', 4, 5.0 ]
```

Проверка наличия в списке:

```python
2 in [1, 2, 3]
# True
```

Сохранение списков в переменные:

```python
# the same list in 3 ways
l = [0, 1, 2, 3, 4]
l = list(range(5))
l = [x for x in range(5)]
```

---
### Оператор if

Условные операторы `if`, `else`, `elif`
Операторы обязательно оканчиваются знаком `:`
Тело оператора выделяется ==отступом==

```python
l = [1, 2, 3]
num = 2

if num in l: 
	print("Number in list")
else:
	print("Number not in list")
```

```python
if today == 'Saturday': 
	print("Party!!!")
elif today == 'Sunday':
	print("Recover.")
else:
	print('Work, work, work.')
```

---
### Цикл for

Тело цикла отделяется ==отступом==. Условие оканчивается знаком `:`

```python
for i in [1, 2, 3]:
	print(i)
# 1
# 2
# 3
```

```python
for ch in "Hi!":
	print(ch)
# H
# i
# !
```

---
### Получение справки

#### dir()

Функция `dir()` позволяет получить информацию о всех атрибутах объекта, включая модули:

```python
import datetime

dir(datetime)
# ['MAXYEAR', 'MINYEAR', '__all__', '__builtins__', '__cached__', '__doc__', '__file__', '__loader__', '__name__', '__package__', '__spec__', 'date', 'datetime', 'datetime_CAPI', 'sys', 'time', 'timedelta', 'timezone', 'tzinfo']

dir(datetime.datetime)
# ['__add__', '__class__', '__delattr__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__ne__', '__new__', '__radd__', '__reduce__', '__reduce_ex__', '__repr__', '__rsub__', '__setattr__', '__sizeof__', '__str__', '__sub__', '__subclasshook__', 'astimezone', 'combine', 'ctime', 'date', 'day', 'dst', 'fold', 'fromisocalendar', 'fromisoformat', 'fromordinal', 'fromtimestamp', 'hour', 'isocalendar', 'isoformat', 'isoweekday', 'max', 'microsecond', 'min', 'minute', 'month', 'now', 'replace', 'resolution', 'second', 'strftime', 'strptime', 'time', 'timestamp', 'timetuple', 'timetz', 'today', 'toordinal', 'tzinfo', 'tzname', 'utcfromtimestamp', 'utcnow', 'utcoffset', 'utctimetuple', 'weekday', 'year']
```

#### help()

Функция `help()` показывает документацию объекта или модуля:

```python
import datetime

help(datetime)
# NAME
#     datetime - Fast implementation of the datetime type.
# ...

help(datetime.datetime)
# Help on class datetime in module datetime:
# 
# class datetime(date)
# ...
```

---
### Функция range()

Функция `range()` возвращает объект типа `range`, который может быть использован для чего-либо:

```python
range(5)
# 0, 1, 2, 3, 4
range(0, 5)
# 0, 1, 2, 3, 4
range(1, 10, 2)
# 1, 3, 5, 7, 9
range(5, 0, -1)
# 5, 4, 3, 2, 1
```

> [!warning] Конечное значение не входит в диапозон

---
### Дополнительный код

```python
import random
import time

rand.randint(0, 5)
rand.randint(1, 60)
time.sleep(1)
```

---

[[Head First Python|Назад]]  [[Chapter 1 - Basics|Вверх]]

#study/python #python/headfirst 

