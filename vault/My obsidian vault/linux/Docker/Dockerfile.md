# Dockerfile: полное руководство

1. [[#Что такое Dockerfile]]
2. [[#Структура и инструкции]]
	1. [[#FROM — базовый образ]]
	2. [[#WORKDIR — рабочая директория]]
	3. [[#COPY и ADD — копирование файлов]]
	4. [[#RUN — выполнение команд при сборке]]
	5. [[#CMD и ENTRYPOINT — команда запуска]]
	6. [[#ENV — переменные окружения]]
	7. [[#EXPOSE — документирование портов]]
	8. [[#VOLUME — точки монтирования]]
	9. [[#USER — смена пользователя]]
	10. [[#ARG — аргументы сборки]]
	11. [[#HEALTHCHECK — проверка здоровья]]
	12. [[#LABEL — метаданные]]
3. [[#Multi-stage сборка]]
4. [[#Лучшие практики]]
	1. [[#Кэширование слоёв]]
	2. [[#Минимизация слоёв]]
	3. [[#.dockerignore]]
	4. [[#Безопасность]]
	5. [[#Отладка и логирование]]
5. [[#Полный пример production-ready Dockerfile]]
6. [[#Справочная таблица инструкций]]
7. [[#Полезные команды]]

---

## Что такое Dockerfile

**Dockerfile** — это текстовый файл с инструкциями для сборки Docker-образа. Каждая инструкция создаёт новый слой образа, который кэшируется для ускорения последующих сборок.

---

## Структура и инструкции

### FROM — базовый образ

```dockerfile
# Базовый синтаксис
FROM <образ>[:<тег>] [AS <имя_этапа>]

# Примеры
FROM ubuntu:22.04
FROM python:3.11-slim
FROM node:20-alpine
FROM scratch  # пустой образ (для статических бинарников)
```

**Рекомендации по выбору базового образа:**

| Тип образа | Размер | Безопасность | Когда использовать |
|------------|--------|--------------|-------------------|
| `ubuntu:22.04` | ~77 MB | Средняя | Нужны apt-пакеты |
| `python:3.11-slim` | ~120 MB | Средняя | Python-приложения |
| `alpine:3.18` | ~7 MB | Высокая | Минимальный размер |
| `distroless` | ~2 MB | Очень высокая | Продакшен (только рантайм) |
| `scratch` | 0 B | Максимальная | Статические бинарники (Go, Rust) |

---

### WORKDIR — рабочая директория

```dockerfile
# Устанавливает рабочую директорию для последующих инструкций
WORKDIR /app

# Можно указывать несколько раз (путь накапливается)
WORKDIR /app
WORKDIR src  # теперь /app/src

# Переменные окружения работают
ENV APP_DIR=/app
WORKDIR ${APP_DIR}
```

---

### COPY и ADD — копирование файлов

```dockerfile
# COPY — предпочтительный вариант
COPY <источник> <назначение>

# Примеры
COPY requirements.txt /app/
COPY . /app                    # всё из контекста сборки
COPY src/ /app/src/            # конкретную директорию
COPY --chown=appuser:appgroup config.json /app/

# ADD — имеет доп. возможности (распаковка архивов, URL)
ADD https://example.com/file.tar.gz /tmp/  # скачает и распакует
ADD archive.tar.gz /app/                    # распакует в /app

# ⚠️ ADD менее предсказуем — используйте COPY, если не нужна распаковка
```

**Важно о контексте сборки:**
```
myproject/
├── Dockerfile
├── .dockerignore
├── src/
└── tests/

# Команда: docker build .
# Контекст = вся папка myproject/
# COPY . /app  → копирует ВСЁ из контекста
```

---

### RUN — выполнение команд при сборке

```dockerfile
# Форм 2: exec (предпочтительно)
RUN ["apt-get", "update"]

# Форм 1: shell (создаёт лишний слой /bin/sh -c)
RUN apt-get update && apt-get install -y python3

# ✅ ХОРОШО: объединяем команды и чистим кэш
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        python3 \
        python3-pip \
    && rm -rf /var/lib/apt/lists/*

# ❌ ПЛОХО: каждый RUN создаёт новый слой
RUN apt-get update
RUN apt-get install -y python3
RUN rm -rf /var/lib/apt/lists/*
```

---

### CMD и ENTRYPOINT — команда запуска

| Инструкция | Когда переопределяется | Назначение |
|------------|----------------------|------------|
| `CMD` | Да (`docker run image command`) | Команда по умолчанию |
| `ENTRYPOINT` | Нет (только с `--entrypoint`) | Основной исполняемый файл |

```dockerfile
# CMD — команда по умолчанию
CMD ["python", "app.py"]
# Можно переопределить: docker run myapp bash

# ENTRYPOINT — фиксированная команда
ENTRYPOINT ["python"]
CMD ["app.py"]  # аргументы по умолчанию
# docker run myapp           → python app.py
# docker run myapp script.py → python script.py

# ✅ ХОРОШО для CLI-утилит
ENTRYPOINT ["kubectl"]
CMD ["--help"]

# ❌ ПЛОХО: shell-форм (сигналы не доходят до процесса)
CMD python app.py  # процесс будет PID 1, но через sh
```

---

### ENV — переменные окружения

```dockerfile
# Форм 1: одна переменная
ENV APP_HOME=/app

# Форм 2: несколько переменных
ENV APP_ENV=production \
    PORT=8080 \
    DEBUG=false

# Использование
WORKDIR ${APP_HOME}
RUN echo "App runs in ${APP_ENV} mode"

# ⚠️ Не хранить секреты!
# ❌ ПЛОХО
ENV DB_PASSWORD=supersecret

# ✅ ХОРОШО — передавать при запуске
# docker run -e DB_PASSWORD=secret myapp
```

---

### EXPOSE — документирование портов

```dockerfile
# Не открывает порт автоматически! Только документация
EXPOSE 80
EXPOSE 443/tcp
EXPOSE 8080/udp

# Запуск с пробросом портов
docker run -p 8080:80 myapp  # хост:контейнер
```

---

### VOLUME — точки монтирования

```dockerfile
# Создаёт точку для персистентных данных
VOLUME ["/data"]
VOLUME /var/log /var/lib/mysql

# При запуске Docker создаст anonymous volume
docker run myapp  # данные сохранятся после удаления контейнера
```

---

### USER — смена пользователя

```dockerfile
# Создание пользователя
RUN useradd -m -u 1000 appuser

# Смена пользователя для последующих инструкций
USER appuser

# ✅ ХОРОШО: запуск от не-root
FROM python:3.11-slim
RUN useradd -m -u 1000 appuser
WORKDIR /app
COPY --chown=appuser:appuser . .
USER appuser
CMD ["python", "app.py"]
```

---

### ARG — аргументы сборки

```dockerfile
# Аргументы доступны ТОЛЬКО при сборке
ARG VERSION=1.0
ARG BUILD_DATE

# Использование
LABEL version=${VERSION}
RUN echo "Built on ${BUILD_DATE}"

# Передача при сборке
docker build --build-arg VERSION=2.0 --build-arg BUILD_DATE=2024-01-01 .

# ⚠️ ARG виден в docker history — не для секретов!
```

**ARG vs ENV:**
```dockerfile
ARG API_KEY      # только при сборке
ENV API_KEY      # сохраняется в образе и доступна в контейнере
```

---

### HEALTHCHECK — проверка здоровья

```dockerfile
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
    CMD curl -f http://localhost:8080/health || exit 1

# Параметры:
# --interval    — между проверками (default: 30s)
# --timeout     — таймаут (default: 30s)
# --start-period — время на старт (default: 0s)
# --retries     — попыток до unhealthy (default: 3)

# Статусы: starting | healthy | unhealthy | none
docker inspect --format='{{.State.Health.Status}}' container
```

---

### LABEL — метаданные

```dockerfile
LABEL maintainer="dev@example.com"
LABEL version="1.0"
LABEL description="My application"

# Группировка (лучше для читаемости)
LABEL org.opencontainers.image.source="https://github.com/user/repo" \
      org.opencontainers.image.description="My app" \
      org.opencontainers.image.version="1.0"
```

---

## Multi-stage сборка

**Проблема:** Образ содержит инструменты сборки, которые не нужны в рантайме.

**Решение:** Multi-stage Dockerfile.

```dockerfile
# Этап 1: сборка
FROM golang:1.21 AS builder

WORKDIR /app
COPY go.mod go.sum ./
RUN go mod download

COPY . .
RUN CGO_ENABLED=0 GOOS=linux go build -o myapp

# Этап 2: рантайм (минимальный образ)
FROM alpine:3.18

RUN apk --no-cache add ca-certificates
WORKDIR /root/
COPY --from=builder /app/myapp .

# Итоговый размер: ~10 MB вместо ~1 GB
CMD ["./myapp"]
```

**Пример для Node.js:**
```dockerfile
# Этап 1: сборка
FROM node:20 AS builder

WORKDIR /app
COPY package*.json ./
RUN npm ci

COPY . .
RUN npm run build

# Этап 2: продакшен
FROM node:20-alpine

WORKDIR /app
COPY --from=builder /app/dist ./dist
COPY --from=builder /app/node_modules ./node_modules
COPY package.json .

USER node
CMD ["node", "dist/index.js"]
```

---

## Лучшие практики

### Кэширование слоёв

```dockerfile
# ✅ ХОРОШО: зависимости копируем первыми (меняются реже)
COPY requirements.txt .
RUN pip install -r requirements.txt

COPY . .

# ❌ ПЛОХО: код копируем до зависимостей
COPY . .
COPY requirements.txt .
RUN pip install -r requirements.txt
# Любое изменение кода инвалидирует кэш pip install
```

### Минимизация слоёв

```dockerfile
# ✅ ХОРОШО: одна инструкция RUN
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        pkg-config \
        libssl-dev \
    && rm -rf /var/lib/apt/lists/*

# ❌ ПЛОХО: много инструкций
RUN apt-get update
RUN apt-get install -y pkg-config
RUN apt-get install -y libssl-dev
```

### .dockerignore

```
# .dockerignore
.git
.gitignore
__pycache__
*.pyc
*.pyo
*.pyd
venv/
.env
*.log
.DS_Store
Thumbs.db
*.md
!requirements.txt  # кроме этого
```

### Безопасность

```dockerfile
# ✅ Полная версия образа
FROM python:3.11.7-slim

# ✅ Проверка уязвимостей
RUN pip install --no-cache-dir safety bandit

# ✅ Запуск от не-root
RUN useradd -m -u 1000 appuser
USER appuser

# ✅ Минимальные привилегии
FROM gcr.io/distroless/python3
COPY --chown=nonroot:nonroot . /app
WORKDIR /app
USER nonroot
CMD ["main.py"]
```

### Отладка и логирование

```dockerfile
# ✅ Явные теги
FROM python:3.11.7-slim  # не :latest!

# ✅ Метки для отслеживания
LABEL version="1.0.0"
LABEL scan_date="2024-01-15"

# ✅ Health check
HEALTHCHECK --interval=30s --timeout=3s \
    CMD curl -f http://localhost:8080/health || exit 1
```

---

## Полный пример production-ready Dockerfile

```dockerfile
# syntax=docker/dockerfile:1

# ============================================
# Этап 1: Сборка
# ============================================
FROM python:3.11-slim AS builder

ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PIP_NO_CACHE_DIR=1 \
    PIP_DISABLE_PIP_VERSION_CHECK=1

WORKDIR /app

# Зависимости
COPY requirements.txt .
RUN pip wheel --no-cache-dir --wheel-dir /wheels -r requirements.txt

# Исходный код
COPY . .

# Тесты (опционально)
RUN pip install pytest && \
    python -m pytest tests/ || true

# ============================================
# Этап 2: Рантайм
# ============================================
FROM python:3.11-slim AS runtime

# Метаданные
LABEL maintainer="dev@example.com" \
      version="1.0.0" \
      org.opencontainers.image.source="https://github.com/user/repo"

# Переменные окружения
ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    APP_HOME=/app \
    PORT=8000

# Пользователь
RUN useradd -m -u 1000 appuser

WORKDIR ${APP_HOME}

# Копируем колёса из builder
COPY --from=builder /wheels /wheels
COPY --from=builder /app/requirements.txt .

RUN pip install --no-cache /wheels/* && \
    rm -rf /wheels

# Копируем приложение
COPY --chown=appuser:appuser . .

# Health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=10s --retries=3 \
    CMD python -c "import urllib.request; urllib.request.urlopen('http://localhost:${PORT}/health')" || exit 1

# Смена пользователя
USER appuser

# Порт
EXPOSE ${PORT}

# Запуск
CMD ["gunicorn", "--bind", "0.0.0.0:8000", "--workers", "4", "app:app"]
```

---

## Справочная таблица инструкций

| Инструкция    | Описание                      | Сохраняется в образе    |
| ------------- | ----------------------------- | ----------------------- |
| `FROM`        | Базовый образ                 | Да                      |
| `RUN`         | Команда при сборке            | Да (как слой)           |
| `CMD`         | Команда по умолчанию          | Да                      |
| `ENTRYPOINT`  | Основной исполняемый файл     | Да                      |
| `COPY`        | Копирование файлов            | Да                      |
| `ADD`         | Копирование с распаковкой/URL | Да                      |
| `ENV`         | Переменные окружения          | Да                      |
| `ARG`         | Аргументы сборки              | Нет (только метаданные) |
| `WORKDIR`     | Рабочая директория            | Да                      |
| `USER`        | Пользователь                  | Да                      |
| `EXPOSE`      | Порт (документация)           | Да                      |
| `VOLUME`      | Точка монтирования            | Да                      |
| `HEALTHCHECK` | Проверка здоровья             | Да                      |
| `LABEL`       | Метаданные                    | Да                      |
| `STOPSIGNAL`  | Сигнал остановки              | Да                      |
| `ONBUILD`     | Триггеры для дочерних образов | Да                      |

---

## Полезные команды

```bash
# Сборка
docker build -t myapp:1.0 .
docker build --no-cache -t myapp:1.0 .  # без кэша
docker build --build-arg VAR=value -t myapp:1.0 .

# Просмотр
docker images                    # список образов
docker history myapp:1.0         # история слоёв
docker inspect myapp:1.0         # детальная информация

# Анализ размера
docker image inspect myapp:1.0 --format='{{.Size}}'
docker buildx du                 # использование кэша

# Очистка
docker system prune -a           # удалить всё неиспользуемое
docker builder prune             # очистить кэш сборки
```

---

[[Docker|Назад]]

#linux/docker #dockerfile
