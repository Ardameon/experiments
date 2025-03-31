1. [[#Основные характеристики LDAP]]
2. [[#Архитектура LDAP]]
	1. [[#Основные компоненты]]
	2. [[#Пример структуры DIT]]
	3. [[#Строковые атрибуты]]
	4. [[#ObjectClass]]
	5. [[#DN - Distinguished Name]]
	6. [[#LDIF - LDAP Data Interchange Format]]
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

| Компонент                            | Описание                                                                                                                                                                                                                                                                                  |
| ------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **DIT (Directory Information Tree)** | Иерархическая структура данных (например, `dc=example,dc=com`) в древовидном виде (как файловая сисетма). Каждай запись в ней, кроме самой верхней, имеет единственную родительскую запись                                                                                                |
| **Entry (Запись)**                   | Элемент каталога (например, пользователь `cn=user1,ou=users,dc=example,dc=com`).                                                                                                                                                                                                          |
| **Attribute (Атрибут)**              | Поле записи (например, `mail=user@example.com`).                                                                                                                                                                                                                                          |
| **ObjectClass**                      | Составная сущность для набора атрибутов. Требуется для упрощения добавления атрибутов в запись (например, objectClass=person, содержит атрибуты: sn, cn, userPassword, telephoneNumber, seeAlso, description) (см. [RFC4519](https://datatracker.ietf.org/doc/html/rfc4519#section-3.12)) |
| **Scheme (Схема)**                   | Группировка определений атрибутов и objectClass'ов. Может быть использована для добавления этих определений в словарь сервера LDAP.                                                                                                                                                       |
| **LDAP Server**                      | Сервер, хранящий данные (OpenLDAP, Active Directory, 389 Directory Server).                                                                                                                                                                                                               |
| **LDAP Client**                      | Программа или библиотека, отправляющая запросы (например, `ldapsearch`).                                                                                                                                                                                                                  |
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

Остальные атрибуты описаны в [RFC4519](https://datatracker.ietf.org/doc/html/rfc4519)

---
### ObjectClass

Класс аттрибутов. Может быть двух типов:
- STRUCTURAL
- AUXILARY

```ldap
objectclass ( 2.5.6.6 NAME 'person'
	         SUP top
	         STRUCTURAL
	         MUST ( sn $
	               cn )
	         MAY ( userPassword $
	               telephoneNumber $
	               seeAlso $ description ) )
```

Обязательность наличия атррибутов определяется спецификаторами:
- MUST
- MAY

Может наследовать атрибуты других классов того же типа с помощью директивы SUP:

```ldap
objectclass ( 2.5.6.7 NAME 'organizationalPerson' SUP person STRUCTURAL
 . . .
```

Класс inetOrgPerson [rfc2798](https://datatracker.ietf.org/doc/html/rfc2798#section-3) :

```ldap
( 2.16.840.1.113730.3.2.2
    NAME 'inetOrgPerson'
    SUP organizationalPerson
    STRUCTURAL
    MAY (
        audio $ businessCategory $ carLicense $ departmentNumber $
        displayName $ employeeNumber $ employeeType $ givenName $
        homePhone $ homePostalAddress $ initials $ jpegPhoto $
        labeledURI $ mail $ manager $ mobile $ o $ pager $
        photo $ roomNumber $ secretary $ uid $ userCertificate $
        x500uniqueIdentifier $ preferredLanguage $
        userSMIMECertificate $ userPKCS12
    )
)
```

имеет следующую иерархию наследования:

```
inetOrgPerson -> organizationalPerson -> person -> top
```

**top** - это абстрактный объектный класс, нужный только для задания объектного класса вершины цепочки наследования

---
### DN - Distinguished Name

`DN`- уникальное имя, которое точно определяет запись(`entry`) в каталоге LDAP.
`DN` состоит из одного или нескольких относительных уникальных имён(`RDN`), разделённых запятой. Относительное уникальное имя имеет вид ИмяАтрибута=значение.

> [!example]
> ```ldap
> dn: cn=Петр Иванов,ou=Менеджеры,dc=domain,dc=com
> ```

---
### LDIF - LDAP Data Interchange Format

Пример записи отображаемой в LDIF:

```ldap
dn: sn=Ellingwood,ou=people,dc=digitalocean,dc=com
objectclass: person
sn: Ellingwood
cn: Justin Ellingwood
```

```ldap
dn: uid=jsmith1,ou=People,dc=example,dc=com
objectClass: inetOrgPerson
cn: John Smith
sn: Smith
uid: jsmith1
```

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