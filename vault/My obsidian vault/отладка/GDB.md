1. [[#Вывод массивов]]
### Вывод массивов

> [!question] Как вывести содержимаое массива `char` длиной `96` байт в виде `hex`-значений?
> ```dbg
> p/x *(uint8_t*)msglenbuf@96
> ```

> [!question] Как сделать вывод каждого эелемента в отдельной строке?
> ```dbg
> set print array on
> ```

> [!question] Как сделать неограниченный вывод массивов?
> ```dbg
> set print elements unlimited
> ```

--- 
#debug/gdb