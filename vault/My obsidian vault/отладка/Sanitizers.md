### Полезные ссылки
1. [GitHub Google](https://github.com/google/sanitizers/wiki)
2. [Статья на хабре](https://habr.com/ru/companies/stc_spb/articles/776528/)
3. [Статья HOWTO](https://www.osc.edu/resources/getting_started/howto/howto_use_address_sanitizer)

---
### AddressSanitizer

Сборка с AdressSanitizer:

> [!warning]- Порядок указания файла  в строке компиляции может быть важен
> Если видишь ошибку вроде 
> ```bash
> /mnt/sdd1 # ./main ./main: /lib//libm.so.6: version `GLIBC_2.23' not found (required by ./main)
> ```
> То попробуй изменить место файла исходников так, чтобы флаги санитайзера были перед ним

```bash
gcc user_after_free.c -fsanitize=address -static-libasan -g
```
```bash
gcc user_after_free.c -fsanitize=address -static-libasan -fno-omit-frame-pointer -g 
```

> [!example]- Пример кода с использованием освобождённой памяти
> ```c
>#include <stdlib.h>
>int main() {
> 	char *x = (char*)malloc(10 * sizeof(char*));
>     free(x);
>     return x[5];
>}
> ```

> [!example]- Пример вывода AddressSanitizer
> ```bash
>=================================================================
>==4438==ERROR: AddressSanitizer: heap-use-after-free on address 0xffff96c019f5 at pc 0xaaaae50d0980 bp 0xffffc04b2e00 sp 0xffffc04b2e10
> READ of size 1 at 0xffff96c019f5 thread T0
>    #0 0xaaaae50d097c in main /home/pylesosik/my/programming/use_after_free.c:5
>    #1 0xffff9a2973f8 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
>    #2 0xffff9a2974c8 in __libc_start_main_impl ../csu/libc-start.c:392
>    #3 0xaaaae50d082c in _start (/home/pylesosik/my/programming/a.out+0x82c)
>
>0xffff96c019f5 is located 5 bytes inside of 80-byte region [0xffff96c019f0,0xffff96c01a40)
>freed by thread T0 here:
>    #0 0xffff9a4c9fe8 in __interceptor_free ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:127
>    #1 0xaaaae50d092c in main /home/pylesosik/my/programming/use_after_free.c:4
>    #2 0xffff9a2973f8 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
>    #3 0xffff9a2974c8 in __libc_start_main_impl ../csu/libc-start.c:392
>    #4 0xaaaae50d082c in _start (/home/pylesosik/my/programming/a.out+0x82c)
>
>previously allocated by thread T0 here:
>    #0 0xffff9a4ca2f4 in __interceptor_malloc ../../../../src/libsanitizer/asan/asan_malloc_linux.cpp:145
>    #1 0xaaaae50d0920 in main /home/pylesosik/my/programming/use_after_free.c:3
>    #2 0xffff9a2973f8 in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
>    #3 0xffff9a2974c8 in __libc_start_main_impl ../csu/libc-start.c:392
>    #4 0xaaaae50d082c in _start (/home/pylesosik/my/programming/a.out+0x82c)
>
>SUMMARY: AddressSanitizer: heap-use-after-free /home/pylesosik/my/programming/use_after_free.c:5 in main
>Shadow bytes around the buggy address:
>  0x200ff2d802e0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>  0x200ff2d802f0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>  0x200ff2d80300: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>  0x200ff2d80310: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>  0x200ff2d80320: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>=>0x200ff2d80330: fa fa fa fa fa fa fa fa fa fa fa fa fa fa[fd]fd
>  0x200ff2d80340: fd fd fd fd fd fd fd fd fa fa fa fa 00 00 00 00
>  0x200ff2d80350: 00 00 00 00 06 fa fa fa fa fa fd fd fd fd fd fd
>  0x200ff2d80360: fd fd fd fa fa fa fa fa fa fa fa fa fa fa fa fa
>  0x200ff2d80370: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>  0x200ff2d80380: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
>Shadow byte legend (one shadow byte represents 8 application bytes):
>  Addressable:           00
>  Partially addressable: 01 02 03 04 05 06 07 
>  Heap left redzone:       fa
>  Freed heap region:       fd
>  Stack left redzone:      f1
>  Stack mid redzone:       f2
>  Stack right redzone:     f3
>  Stack after return:      f5
>  Stack use after scope:   f8
>  Global redzone:          f9
>  Global init order:       f6
>  Poisoned by user:        f7
>  Container overflow:      fc
>  Array cookie:            ac
>  Intra object redzone:    bb
>  ASan internal:           fe
>  Left alloca redzone:     ca
>  Right alloca redzone:    cb
>  Shadow gap:              cc
>==4438==ABORTING
> ```

---
### ThreadSanitizer

Сборка с ThreadSanitizer:
 ```bash
 gcc simple_race.c -g -O1 -fsanitize=thread -ltsan 
```
 ```bash
 gcc simple_race.c -g -O0 -fsanitize=thread -ltsan -fPIE -pie
```

> [!example]- Пример кода с "гонкой" `simple_race.c`
> ```c
> #include <pthread.h>
> #include <stdio.h>
>
>int Global;
>
>void *Thread1(void *x) {
>  Global++;
>  return NULL;
>}
>
>void *Thread2(void *x) {
>  Global--;
>  return NULL;
>}
>
>int main() {
>  pthread_t t[2];
>  pthread_create(&t[0], NULL, Thread1, NULL);
>  pthread_create(&t[1], NULL, Thread2, NULL);
>  pthread_join(t[0], NULL);
>  pthread_join(t[1], NULL);
>}
>```

> [!example]- Пример вывода ThreadSanitizer
> ```bash
> ==================
>WARNING: ThreadSanitizer: data race (pid=4220)
>  Read of size 4 at 0xaaaaaaab2014 by thread T2:
>    #0 Thread2 /home/pylesosik/my/programming/simple_race.c:12 (a.out+0xb70)
>
>  Previous write of size 4 at 0xaaaaaaab2014 by thread T1:
>    #0 Thread1 /home/pylesosik/my/programming/simple_race.c:7 (a.out+0xb1c)
>
>  Location is global 'Global' of size 4 at 0xaaaaaaab2014 (a.out+0x000000012014)
>
>  Thread T2 (tid=4223, running) created by main thread at:
>    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x63bb0)
>    #1 main /home/pylesosik/my/programming/simple_race.c:19 (a.out+0xc20)
>
>  Thread T1 (tid=4222, finished) created by main thread at:
>    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x63bb0)
>    #1 main /home/pylesosik/my/programming/simple_race.c:18 (a.out+0xc00)
>
>SUMMARY: ThreadSanitizer: data race /home/pylesosik/my/programming/simple_race.c:12 in Thread2
>==================
>ThreadSanitizer: reported 1 warnings
>```

---
#debug/sanitizer