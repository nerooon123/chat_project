# TCP Chat Server and Client на C++ (Winsock2)

## О проекте

Проект реализует простой TCP чат на Windows с использованием Winsock2 API.  
Включает многопоточный сервер и клиент, поддерживающие обмен сообщениями.  
Сервер обрабатывает каждого клиента в отдельном потоке, что обеспечивает одновременную работу нескольких клиентов.

---

## Технологии

- C++17  
- Winsock2 (Windows Sockets API)  
- Многопоточность: `std::thread` (возможна замена на WinAPI)  
- Компиляция: MSVC или MinGW-w64 (рекомендуется MSYS2)

---

## Файлы

- `server_win.cpp` — многопоточный TCP сервер  
- `client_win.cpp` — TCP клиент

---

## Сборка

### MSVC (Visual Studio Developer Command Prompt)

```bash
cl server_win.cpp ws2_32.lib
cl client_win.cpp ws2_32.lib
```

### MinGW-w64 (MSYS2)

```bash
g++ -std=c++17 server_win.cpp -o server.exe -lws2_32 -pthread
g++ -std=c++17 client_win.cpp -o client.exe -lws2_32 -pthread
```

---

## Запуск

### 1. Запустите сервер:
```bash
./server.exe
```

### 2. Запустите клиент:
```bash
./client.exe
```

### 3. Введите сообщения в клиенте — они отправятся на сервер и будут отображены в ответ.

### 4. Для выхода в клиенте введите exit.

---

## Особенности
### 1. Асинхронный многопоточный сервер на основе std::thread.
### 2. Используется нативный Windows API — Winsock2.
### 3. Возможность расширения под UDP или передачу файлов.
### 4. Требуется Windows 7 или выше.

---

## Требования
### 1. ОС Windows
### 2. Компилятор с поддержкой C++17 и Winsock2 (MSVC, MinGW-w64/MSYS2)
### 3. Для MinGW-w64 рекомендуется использовать MSYS2 для корректной работы потоков

---

## Лицензия
### MIT License

---

## Контакты
### Автор: Тимур
### GitHub: https://github.com/nerooon123

