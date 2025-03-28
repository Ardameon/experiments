## Полная инструкция по развертыванию LDAP-сервера

### 1. Установка необходимых пакетов

```bash
sudo apt update
sudo apt install slapd ldap-utils
```

При установке система запросит:
- Пароль для администратора LDAP (запомните его)
- Подтверждение пароля

### 2. Перенастройка slapd (если требуется)

Если нужно изменить настройки или вы пропустили этап настройки:

```bash
sudo dpkg-reconfigure slapd
```

Пошаговые ответы на вопросы:
1. Пропустить начальную конфигурацию? → **No**
2. Доменное имя (например: example.com) → введенное значение преобразуется в `dc=example,dc=com`
3. Название организации → например: My Company
4. Пароль администратора → задайте надежный пароль
5. База данных → HDB (оставить по умолчанию)
6. Удалить старую базу при перезаписи? → **No**
7. Переместить старую базу? → **Yes**

### 3. Проверка работы сервера

```bash
sudo systemctl status slapd
```

Ожидаемый вывод:
```
● slapd.service - LSB: OpenLDAP standalone server (Lightweight Directory Access Protocol)
   Loaded: loaded (/etc/init.d/slapd; generated)
   Active: active (running) since ...
```

### 4. Добавление тестовых данных

Создаем файл `test.ldif`:

```ldif
dn: dc=example,dc=com
objectClass: top
objectClass: dcObject
objectClass: organization
o: Example Company
dc: example

dn: ou=people,dc=example,dc=com
objectClass: organizationalUnit
ou: people

dn: ou=groups,dc=example,dc=com
objectClass: organizationalUnit
ou: groups

dn: cn=admin,dc=example,dc=com
objectClass: simpleSecurityObject
objectClass: organizationalRole
cn: admin
description: LDAP administrator
userPassword: {SSHA}hashed_password_here
```

Для генерации хеша пароля используйте:

```bash
slappasswd
```

Добавляем данные в LDAP:

```bash
sudo ldapadd -x -D cn=admin,dc=example,dc=com -W -f test.ldif
```

Система запросит пароль администратора.

### 5. Проверка добавленных данных

```bash
ldapsearch -x -b "dc=example,dc=com" "(objectClass=*)"
```

### 6. Управление сервером

Основные команды:

- Перезапуск сервера:
```bash
sudo systemctl restart slapd
```

- Проверка конфигурации:
```bash
sudo slaptest -u
```

- Включение автозапуска:
```bash
sudo systemctl enable slapd
```
### 7. Дополнительные настройки безопасности

1. Включение TLS:
```bash
sudo apt install gnutls-bin ssl-cert
sudo mkdir /etc/ldap/ssl
sudo certtool --generate-privkey --bits 2048 --outfile /etc/ldap/ssl/ldap.key
sudo certtool --generate-self-signed --load-privkey /etc/ldap/ssl/ldap.key --outfile /etc/ldap/ssl/ldap.crt
```

2. Настройка прав:
```bash
sudo chown openldap:openldap /etc/ldap/ssl/ldap.*
sudo chmod 600 /etc/ldap/ssl/ldap.*
```

3. Редактирование конфига:
```bash
sudo nano /etc/ldap/ldap.conf
```
Добавить строки:
```
TLS_CACERT /etc/ldap/ssl/ldap.crt
TLS_REQCERT allow
```

### 9. Важные замечания

1. Для продакшн-среды:
- Всегда используйте TLS/SSL
- Регулярно делайте бэкапы базы данных
- Настройте firewall для ограничения доступа

2. Пути к важным файлам:
- Основной конфиг: `/etc/ldap/ldap.conf`
- Конфигурация slapd: `/etc/ldap/slapd.d/`
- Базы данных: `/var/lib/ldap/`

3. Полезные команды:
- Проверка подключения: `ldapwhoami -x`
- Удаление записи: `ldapdelete -x -D "cn=admin,dc=example,dc=com" -W "cn=user,ou=people,dc=example,dc=com"`
- Изменение записи: `ldapmodify -x -D "cn=admin,dc=example,dc=com" -W -f changes.ldif`

### 10. Решение возможных проблем

1. Ошибки аутентификации:
- Проверьте правильность DN и пароля
- Убедитесь, что сервер запущен

2. Проблемы с подключением:
- Проверьте порты (389 для LDAP, 636 для LDAPS)
- Убедитесь, что firewall не блокирует соединение

3. Ошибки в данных:
- Проверьте синтаксис LDIF-файлов
- Убедитесь в правильности objectClass

Для более сложных настроек обратитесь к официальной документации OpenLDAP.

#ldap/openldap