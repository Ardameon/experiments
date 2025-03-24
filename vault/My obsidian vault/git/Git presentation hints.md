>[!example]- Вывести локальные ветки в порядке даты изменения
>```bash
>git for-each-ref --sort=committerdate refs/heads/ --format='%(HEAD) %(color:yellow)%(refname:short)%(color:reset) - %(color:red)%(objectname:short)%(color:reset) - %(contents:subject) - %(authorname) (%(color:green)%(committerdate:relative)%(color:reset))'
>```

>[!example]- Вывести коммиты файла по порядку начиная с даты
>```bash
>git log --reverse --pretty=format:"%h %ar %s" --regexp-ignore-case --since="2017-01-26" -- */turn_sock.c --grep TURN
>```

#git/hints #git/branch #git/commit
