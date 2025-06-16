
# Metric Library

Полностью потокобезопасная универсальная библиотека для сбора метрик в фоне.  
Поддерживает регистрацию любых метрик через функцию получения значения.  
Библиотека вызывает все метрики по таймеру и логирует их в файл.

## 📦 Структура проекта

```

metric_Collector/
│
├── metric_lib/
│   ├── Metric.hpp
│   ├── MetricRegistry.hpp
│   ├── MetricLogger.hpp
│   └── MetricLogger.cpp
│
├── example/
│   ├── CPU_HTTP_example.cpp
│   ├── MEMORY_NETWORK_example.cpp
│   ├── Server_temp_Queue_size_example.cpp
│   └── DB_aval_ACT_users_example.cpp

```


## 🚀 Быстрая сборка под Windows (MSYS2 / MinGW)

Запускать команды из корня проекта `metric_Collector/`.

### 1️⃣ CPU + HTTP пример:

```bash
C:\msys64\ucrt64\bin\g++.exe -std=c++17 -Imetric_lib example/CPU_HTTP_example.cpp metric_lib/MetricLogger.cpp -o example/CPU_HTTP_example.exe
```

### 2️⃣ MEMORY + NETWORK пример:

```bash
C:\msys64\ucrt64\bin\g++.exe -std=c++17 -Imetric_lib example/MEMORY_NETWORK_example.cpp metric_lib/MetricLogger.cpp -o example/MEMORY_NETWORK_example.exe
```

### 3️⃣ Server Temperature + Queue Size пример:

```bash
C:\msys64\ucrt64\bin\g++.exe -std=c++17 -Imetric_lib example/Server_temp_Queue_size_example.cpp metric_lib/MetricLogger.cpp -o example/Server_temp_Queue_size_example.exe
```

### 4️⃣ DB Availability + Active Users пример:

```bash
C:\msys64\ucrt64\bin\g++.exe -std=c++17 -Imetric_lib example/DB_aval_ACT_users_example.cpp metric_lib/MetricLogger.cpp -o example/DB_aval_ACT_users_example.exe
```

## 📝 Описание архитектуры

* **Регистрация метрик** производится один раз в `main()` через передачу `function<double()>` (функции получения значения метрики).
* **MetricLogger** работает в отдельном фоне и сам опрашивает все метрики с заданной периодичностью.
* **MetricRegistry** содержит глобальную регистрацию метрик.
* Все метрики полностью потокобезопасны.
* Все примеры используют имитационные симуляторы значений (для демонстрации работы).


## 📊 Поддерживаемые метрики в примерах:

| Example                                | Метрики                            |
| -------------------------------------- | ---------------------------------- |
| CPU\_HTTP\_example.cpp                 | CPU utilization, HTTP requests RPS |
| MEMORY\_NETWORK\_example.cpp           | Memory usage MB, Network KB/s      |
| Server\_temp\_Queue\_size\_example.cpp | Server temperature C, Queue size   |
| DB\_aval\_ACT\_users\_example.cpp      | DB availability, Active users      |


## 🖥 Требования:

* C++17 (обязательно)
* g++ (рекомендуется MSYS2 / MinGW для Windows)
* Однопоточность основной программы полностью не блокируется.
* Полностью потокобезопасная запись лога.


## 📂 Пример вывода в лог (metrics.log):

```
2025-06-16 15:00:02.653 "CPU" 3.12 "HTTP requests RPS" 450
2025-06-16 15:00:03.653 "CPU" 2.45 "HTTP requests RPS" 388
2025-06-16 15:00:04.653 "CPU" 4.07 "HTTP requests RPS" 520
```
