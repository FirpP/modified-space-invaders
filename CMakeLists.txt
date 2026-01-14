cmake_minimum_required(VERSION 3.10)

# Nazwa projektu
project(SpaceInvaders)

# Ustawienie standardu C++ na 11 (zgodnie z poleceniem g++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Znajdź bibliotekę wątków (potrzebna dla -pthread)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Zdefiniuj pliki źródłowe
set(SOURCES
    main1.cpp
    Game.cpp
    Entities.cpp
    ConsoleUtils.cpp
)

# Utwórz plik wykonywalny
add_executable(space_invaders ${SOURCES})

# Podlinkuj bibliotekę wątków
target_link_libraries(space_invaders Threads::Threads)