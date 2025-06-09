
>[!help] Удаление пакета с `unmet dependency`
>```bash
>sudo dpkg --force-all -P libgbm1
>```

> [!help] Восстановить сломанные зависимости
> ```bash
> sudo apt-get -f install
>```

#linux/ubuntu #linux/apt