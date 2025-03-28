1. [[#Основные характеристики LDAP]]
2. [[#Архитектура LDAP]]
	1. [[#Основные компоненты]]
	2. [[#Пример структуры DIT]]
	3. [[#Строковые атрибуты]]
3. [[#Основные операции LDAP]]

## Протокол LDAP: обзор  

**LDAP** (Lightweight Directory Access Protocol) – это протокол прикладного уровня для доступа к службам каталогов (directory services). Он используется для поиска, чтения и изменения данных в иерархических базах данных (например, Active Directory, OpenLDAP).  

[RFC4510](https://datatracker.ietf.org/doc/html/rfc4510)

---
## Основные характеристики LDAP  

- **Легковесный (Lightweight)**: Упрощённая версия протокола X.500 (DAP).  
- **Иерархическая структура**: Данные хранятся в древовидной форме (DIT – Directory Information Tree).  
- **Клиент-серверная модель**: Клиенты отправляют запросы, сервер их обрабатывает.  
- **Поддержка аутентификации**: SASL, SSL/TLS, простой логин/пароль.  
- **Кросс-платформенность**: Работает на Windows (Active Directory), Linux (OpenLDAP), macOS.  

---

## Архитектура LDAP 

### Основные компоненты

| Компонент                            | Описание                                                                         |     |
| ------------------------------------ | -------------------------------------------------------------------------------- | --- |
| **DIT (Directory Information Tree)** | Иерархическая структура данных (например, `dc=example,dc=com`).                  |     |
| **Entry (Запись)**                   | Элемент каталога (например, пользователь `cn=user1,ou=users,dc=example,dc=com`). |     |
| **Attribute (Атрибут)**              | Поле записи (например, `mail=user@example.com`).                                 |     |
| **ObjectClass**                      | Шаблон, определяющий допустимые атрибуты (например, `inetOrgPerson`).            |     |
| **LDAP Server**                      | Сервер, хранящий данные (OpenLDAP, Active Directory, 389 Directory Server).      |     |
| **LDAP Client**                      | Программа или библиотека, отправляющая запросы (например, `ldapsearch`).         |     |
### Пример структуры DIT 
```
dc=example,dc=com  
├── ou=users  
│   ├── cn=user1 (mail: user1@example.com)  
│   └── cn=user2 (mail: user2@example.com)  
└── ou=groups  
    ├── cn=admins  
    └── cn=developers  
```

### Строковые атрибуты

Описание из [RFC4514](https://datatracker.ietf.org/doc/html/rfc4514)

```
 String  X.500 AttributeType
 ------  --------------------------------------------
 CN      commonName (2.5.4.3)
 L       localityName (2.5.4.7)
 ST      stateOrProvinceName (2.5.4.8)
 O       organizationName (2.5.4.10)
 OU      organizationalUnitName (2.5.4.11)
 C       countryName (2.5.4.6)
 STREET  streetAddress (2.5.4.9)
 DC      domainComponent (0.9.2342.19200300.100.1.25)
 UID     userId (0.9.2342.19200300.100.1.1)
```

Остальные атрибуты описаны в [RFC4514](https://datatracker.ietf.org/doc/html/rfc4514)

---
## Основные операции LDAP 
| Операция                  | Описание                   | Пример команды (OpenLDAP)                                                       |
| ------------------------- | -------------------------- | ------------------------------------------------------------------------------- |
| **Bind (Аутентификация)** | Подключение к серверу      | `ldapbind -D "cn=admin,dc=example,dc=com" -w password`                          |
| **Search (Поиск)**        | Поиск записей              | `ldapsearch -b "ou=users,dc=example,dc=com" "(cn=user1)"`                       |
| **Add (Добавление)**      | Создание записи            | `ldapadd -x -D "admin_dn" -w password -f entry.ldif`                            |
| **Modify (Изменение)**    | Обновление атрибутов       | `ldapmodify -x -D "admin_dn" -w password -f changes.ldif`                       |
| **Delete (Удаление)**     | Удаление записи            | `ldapdelete -x -D "admin_dn" -w password "cn=user1,ou=users,dc=example,dc=com"` |
| **Compare (Сравнение)**   | Проверка значения атрибута | Используется в API (например, `ldap_compare()` в C)                             |

---
#ldap