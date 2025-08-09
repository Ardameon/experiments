> [!question] Как удалить служебные файлы VIM `.swp` или BC3 `.orig` , чтобы они случайно не попали в Git-репозиторий?
> ```bash
> rm `git st | grep ".swp\|.orig" | tr -d "\n"`
>```

#git/hints #bash/utils #linux #vim