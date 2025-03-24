>[!example]- Создать 1 патч из коммита \<sha\> 
>```bash
>git format-patch -1 \<sha\>
>```

>[!example]- Применить патч в тот же репозиторий 
>```bash
>git apply --3way 0001-some-changes.patch
>```
 
>[!example]- Применить патч в другой репозиторий (продвинутый вариант) 
>```bash
>git apply -p4 --directory='_eltex/drivers/board/' --3way 0001-smg2016-drivers-refs-179108-x25-switch-channel-mode-.patch
>```

>[!example]- Посмотреть что будет изменено 
>```bash
>git apply --stat my-patch.patch
>```

>[!example]- Проверить установится ли патч 
>```bash
>git apply --check my-patch.patch
>```

>[!example]- Применить патч без формирования коммита 
>```bash
>git apply my-patch.patch
>```

>[!example]- Применить патч с коммитом 
>```bash
>git am --signoff < my-patch.patch
>```

>[!warning]
> Чтобы сделать `git am`, нужно чтобы патч применялся при `-p1`, если `-p` требуется больше чем 1, то нужно отредактировать 
> пути в самом патче

#git/patch