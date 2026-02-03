Модули CMake обеспечивают, как и везде, переиспользование уже готового кода (модулей).

Подробнее можно посмотреть на странице документации [cmake-modules](https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html#manual:cmake-modules(7))

Поиск модулей осуществляется:
- указанием полного пути до модуля
- автоматическим поиском модуля по имени средствами самого CMake

По-умолчанию CMake будет пытаться искать модули в каталогах указанных в `CMAKE_MODULE_PATH`. Если по этим путям ничего не будет найдено, то CMake будет искать в поддиректории сборки `Modules`

Модули делятся на 2 категории:
- Find Modules
- Utility Modules

---
### Find Modules

Модули которые поддерживают команду [find_package()](https://cmake.org/cmake/help/latest/command/find_package.html#command:find_package) Их не следует явно включать в скрипты через `include`. Это могут быть какие-то внешние заголовочные файлы или библиотеки.

---
### Utility Modules

Это по факту кусочки CMake-команд которые непосредственно вставляются в `CMakeLists.txt` через  команду [include()](https://cmake.org/cmake/help/latest/command/include.html#command:include).

Например модуль CMake [CheckTypeSize](https://cmake.org/cmake/help/latest/module/CheckTypeSize.html#module:CheckTypeSize):

```cmake
include(CheckTypeSize)
check_type_size(long SIZEOF_LONG)
```

---
[[CMake|Назад]]

#cmake/modules