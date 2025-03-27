CMakeLists.txt - это текстовый файл, который содержит в себе текст на языке [cmake-language](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#manual:cmake-language(7))

### Комментарии

```cmake
# это комментарий
```

---
### Переменные

> [!note]
> Переменные регистрозависимы.

Есть набор переменных, которые автоматически определяются CMake'ом, 
они начинаются с префикса ==CMAKE_== и описаны в мануале [cmake-variables](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html#manual:cmake-variables(7)).
Например:
```cmake
CMAKE_SOURCE_DIR
CMAKE_BINARY_DIR
```

Значения переменных устанавливаются командой [set](https://cmake.org/cmake/help/latest/command/set.html#command:set). 
>[!note]
>В переменной сохраняется либо одно, либо несколько значений разделённых знаком `;` в виде строки

```cmake
set(Foo "")      # 1 quoted arg -> value is ""
set(Foo a)       # 1 unquoted arg -> value is "a"
set(Foo "a b c") # 1 quoted arg -> value is "a b c"
set(Foo a b c)   # 3 unquoted args -> value is "a;b;c"
```

К переменным можно обратиться через запись `${VAR}`, где `VAR` - имя переменной:
```cmake
set(Foo a b c)    # 3 unquoted args -> value is "a;b;c"
command(${Foo})   # unquoted arg replaced by a;b;c
                  # and expands to three arguments
command("${Foo}") # quoted arg value is "a;b;c"
set(Foo "")       # 1 quoted arg -> value is empty string
command(${Foo})   # unquoted arg replaced by empty string
                  # and expands to zero arguments
command("${Foo}") # quoted arg value is empty string
```

К переменным среды операционной системы можно обратиться через нотацию `ENV${VAR}`:
```cmake
message("System path variable: " $ENV{PATH})
```

---
### Область видимости переменных

Переменная объявленная в CMakeLists.txt видна:
- Данном CMakeLists.txt
- В функциях данного CMakeLists.txt
- Во всех CMakeLists.txt используемых как `subdirectory`
- Во всех файлах файлах добавленных командой [include](https://cmake.org/cmake/help/latest/command/include.html#command:include)

При вызове `subdirectory` или вызове функции создаётся новый контекст переменных, в который копируются переменные вызывающего контекста.
>[!attention]
>Переменные объявленные в новом контексте или изменённые внутри нового контекста переменные вызывающего контекста ==не попадают== в вызывающий контекст автоматически

```cmake
function(foo)
  message(${test}) # test is 1 here
  set(test 2)
  message(${test}) # test is 2 here, but only in this scope
endfunction()

set(test 1)
foo()
message(${test}) # test will still be 1 here
```

Когда нужно поменять значение переменной в вызывающем контексте, в `set` используется опция `PARENT_SCOPE`:
```cmake
function(foo)
  message(${test}) # test is 1 here
  set(test 2 PARENT_SCOPE)
  message(${test}) # test still 1 in this scope
endfunction()

set(test 1)
foo()
message(${test}) # test will now be 2 here
```

Переменные в CMake определяются в порядке выполнения команды `set`:
```cmake
# FOO is undefined

set(FOO 1)
# FOO is now set to 1

set(FOO 0)
# FOO is now set to 0
```

```cmake
set(foo 1)

# process the dir1 subdirectory 
add_subdirectory(dir1) # foo is defined in dir1, bar is not

# include and process the commands in file1.cmake
include(file1.cmake) # foo is defined in file1.cmake, bar is not

set(bar 2)
# process the dir2 subdirectory
add_subdirectory(dir2) # foo and bar are defined in dir2

# include and process the commands in file2.cmake
include(file2.cmake) # foo and bar are defined in file2.cmake
```

---
### Команды

Команды CMake состоят из:
- Имени команды (регистронезависимого)
- Открывающейся скобки `(`
- Разделённых пробелами аргументов
- Закрывающейся скобки `)`

Команды описаны с мануале [cmake-commands](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html#manual:cmake-commands(7))

```cmake
command("")          # 1 quoted argument
command("a b c")     # 1 quoted argument
command("a;b;c")     # 1 quoted argument
command("a" "b" "c") # 3 quoted arguments
command(a b c)       # 3 unquoted arguments
command(a;b;c)       # 1 unquoted argument expands to 3
command(\"abc\")     # 1 unquoted argument
```

---
### Базовые команды

- `set()` и `unset()`- команды задания и удаления переменных
- `list()` - операции со списками
- `string()` - операции со строкам
- `separate_arguments()` - распарсить аргументы командной строки в список

