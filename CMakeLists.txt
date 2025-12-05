cmake_minimum_required(VERSION 3.10.0)
cmake_minimum_required(VERSION 3.10)

# Име на целия проект
project(ClientServerApp)

# Задаваме C++ стандарт (препоръчително C++17 или по-нов)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- 1. Конфигурация за Клиента ---
# Създава изпълним файл "client_app" от сорс кода в Client/main.cpp
add_executable(client_app Client/main.cpp)

