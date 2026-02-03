### Необходимые материалы

Всю информацию можно найти по ссылке на [сайте флеймграфа](https://www.brendangregg.com/flamegraphs.html)
Все необходимые скрипты можно найти в репозитории [FlameGraph](https://github.com/brendangregg/FlameGraph/tree/master)

> [!attention]
> Для примера необходимо наличие скриптов [stackcollapse-perf.pl](https://github.com/brendangregg/FlameGraph/blob/master/stackcollapse-perf.pl) и [flamegraph.pl](https://github.com/brendangregg/FlameGraph/blob/master/flamegraph.pl).

---
### Инструкция

 1. Устанавливаем `perf` для данной версии ядра Linux:

```bash
sudo apt install linux-tools-KERNEL_VERSION-generic
```

Пример:

```bash
$ uname -r
6.8.0-52-generic

$ apt install linux-tools-6.8.0-52-generic
```

2. Собираем бинарник с опцией компилятора `-fno-omit-frame-pointer` и отладочными символами
3. Запускаем бинарник, смотрим какой у него PID
4. Запускаем `perf` и через необходимое время останавливаем его. В результате получаем файл отчёта perf:

```bash
sudo perf record -F99 -g -e cpu-clock -p PROCESS_PID
```

> [!note]
> `perf` можно запустить не по самому процессу, а например по конкретному ядру процессора, чтобы получить картину по всем процессам на ядре (см. `man perf-record`)

5. Запускаем скрипты для формирования flamegraph:

```bash
sudo perf script > out.perf
stackcollapse-perf.pl out.perf > out.folded
flamegraph.pl out.folded > out.svg
```

6. Получаем сам флеймграф - `out.svg`. Открываем его в браузере, наслаждаемся. 

---
[[Отладка|Назад]]

#debug/profiling #flamegraph #perf
