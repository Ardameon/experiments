1. [[#Комментарии]]
2. [[#Переменные]]
3. [[#Область видимости переменных]]
4. [[#Команды]]
	1. [[#Базовые команды]]
	2. [[#Главные команды]]
5. [[#Управление ходом выполнения]]
	1. [[#Условный оператор]]
	2. [[#Циклы]]
	3. [[#Определение процедур]]
6. [[#Регулярные выражение]]
7. [[#Продвинутые команды]]

---

CMakeLists.txt - это текстовый файл, который содержит в себе текст на языке [cmake-language](https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#manual:cmake-language(7))

---
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
#### Базовые команды

- `set()` и `unset()`- команды задания и удаления переменных
- `list()` - операции со списками
- `string()` - операции со строкам
- `separate_arguments()` - распарсить аргументы командной строки в список

---
#### Главные команды

- `add_executable()` - определяет сборку исполняемых файлов
- `add_library()` - определяет сборку библиотеки

Обе команды в качестве параметров принимают списки файлов исходников.

> [!attention]- В список параметров следует включать заголовочные файлы
> Если заголовочные файлы не включить в список параметров этих команд, то CMake не будет отслеживать их изменения и не будет добавлять их в файлы проекта IDE.

---
### Управление ходом выполнения 

CMake поддерживает 3 конструкции для управления ходом выполнения сборки:

- Условный оператор - `if()`
- Циклы - `foreach()` и `while()`
- Определение процедур - `macro()` и `function()`

---
#### Условный оператор

```cmake
if(MSVC80)
  # do something here
elseif(MSVC90)
  # do something else
elseif(APPLE)
  # do something else
else()
  # do something else
endif()
```

> [!note]- Константные выражения истинности используемые под условием
>TRUE-константы: `1, TRUE, ON, YES, Y, non-zero-number` 
>FALSE-константы: `0, FALSE, OFF, NO, N, IGNORE, NOTFOUND`

> [!attention]
>Условие считается `TRUE` если оно является:
>- TRUE-константой
>- Строкой обозначающей TRUE-константу
>- Определённой переменной со значением отличным от FALSE-констант

> [!example]- Варианты логических проверок и проверок существования:
```cmake
if(NOT <condition>)
if((condition) AND (condition OR (condition)))
if(COMMAND <command-name>)
if(TARGET <target-name>)
if(DEFINED <name>|CACHE{<name>}|ENV{<name>})
if(<variable|string> IN_LIST <variable>)
if(EXISTS <path-to-file-or-directory>)
if(IS_READABLE <path-to-file-or-directory>)
if(IS_DIRECTORY <path>)
```

Подробнее описано в [мануале](https://cmake.org/cmake/help/latest/command/if.html#command:if)

---
#### Циклы

- `foreach()` - цикл для перебора значений внутри списка
- `while()` - цикл с проверкой условия
- `break()` и `continue()` - команды воздействия на цикл

> [!note] Первый аргумент цикла `foreach()` - переменная для элемента списка

```cmake
foreach(tfile
        TestAnisotropicDiffusion2D
        TestButterworthLowPass
        TestButterworthHighPass
        TestCityBlockDistance
        TestConvolve
        )
  add_test(${tfile}-image ${VTK_EXECUTABLE}
    ${VTK_SOURCE_DIR}/Tests/rtImageTest.tcl
    ${VTK_SOURCE_DIR}/Tests/${tfile}.tcl
    -D ${VTK_DATA_ROOT}
    -V Baseline/Imaging/${tfile}.png
    -A ${VTK_SOURCE_DIR}/Wrapping/Tcl
    )
endforeach()
```

```cmake
while(${CTEST_ELAPSED_TIME} LESS 36000)
  set(START_TIME ${CTEST_ELAPSED_TIME})
  ctest_run_script("dash1_ParaView_vs71continuous.cmake")
  ctest_run_script("dash1_cmake_vs71continuous.cmake")
endwhile()
```

---
#### Определение процедур

- `function()` - функция, создаёт новый вложенный контекст
- `macro()` - макрос, не создаёт контекст (см. [[#Область видимости переменных]])

В процедурах определены следующие переменные

- `ARGC` - кол-во параметров
- `ARGV` - список всех параметров
- `ARGV0.. ARGVN` - параметры
- `ARGN` - список всех параметров после формальных (описанных в объявлении процедуры)

```cmake
function(DetermineTime _time)
  # pass the result up to whatever invoked this
  set(${_time} "1:23:45" PARENT_SCOPE)
endfunction()

# now use the function we just defined
DetermineTime(current_time)

if(DEFINED current_time)
  message(STATUS "The time is now: ${current_time}")
endif()
```

```cmake
# define a simple macro
macro(assert TEST COMMENT)
  if(NOT ${TEST})
    message("Assertion failed: ${COMMENT}")
  endif()
endmacro()

# use the macro
find_library(FOO_LIB foo /usr/local/lib)
assert(${FOO_LIB} "Unable to find library foo")
```

```cmake
# define a macro that takes at least two arguments
# (the formal arguments) plus an optional third argument
macro(assert TEST COMMENT)
  if(NOT ${TEST})
    message("Assertion failed: ${COMMENT}")

    # if called with three arguments then also write the
    # message to a file specified as the third argument
    if(${ARGC} MATCHES 3)
      file(APPEND ${ARGV2} "Assertion failed: ${COMMENT}")
    endif()

  endif()
endmacro()

# use the macro
find_library(FOO_LIB foo /usr/local/lib)
assert(${FOO_LIB} "Unable to find library foo")
```

>[!attention] 
>Для возвращения из функции, директории или файла можно использовать команду `return()`

---
### Регулярные выражение

Работа с регулярками описана в мануале на коменду [string()](https://cmake.org/cmake/help/latest/command/string.html#search-and-replace-with-regular-expressions)

---

### Продвинутые команды

- [`add_dependencies`](https://cmake.org/cmake/help/latest/command/add_dependencies.html#command:add_dependencies "(in CMake v3.30.3)")
- [`include_regular_expression`](https://cmake.org/cmake/help/latest/command/include_regular_expression.html#command:include_regular_expression "(in CMake v3.30.3)")

Общая информация по [ссылке](https://cmake.org/cmake/help/book/mastering-cmake/chapter/Writing%20CMakeLists%20Files.html#advanced-commands)

---
#cmake/masteringcmake 