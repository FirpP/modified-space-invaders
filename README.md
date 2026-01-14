## Space Invaders - C++ Terminal game

Klasyczna gra zręcznościowa **Space Invaders** zaimplementowana w nowoczesnym C++ (standard C++11). Projekt działa w terminalu systemowym (Linux/macOS/Windows) i demonstruje wykorzystanie programowania obiektowego (OOP) oraz zarządzania pamięcią w środowisku tekstowym.

## Zasady gry i Sterowanie

Celem gry jest obrona Ziemi przed inwazją obcych. Musisz zestrzelić wszystkich przeciwników, zanim dotrą do dolnej krawędzi ekranu.

## Klawiszologia

| **Klawisz** | **Akcja** |
| --- | --- |
| **A** | Ruch w lewo |
| --- | --- |
| **D** | Ruch w prawo |
| --- | --- |
| **SPACJA** | Strzał |
| --- | --- |
| **Q** | Wyjście z gry (Quit) |
| --- | --- |

## Warunki zwycięstwa i porażki

- **Zwycięstwo:** Zestrzelenie wszystkich obcych na planszy (gra resetuje formację obcych i gra toczy się dalej).
- **Porażka (game Over):** Jeśli jakikolwiek obcy dotknie dolnej krawędzi ekranu (linii obrony gracza).

## Jak uruchomić projekt

Projekt można zbudować na dwa sposoby. Zalecaną metodą jest użycie **CMake**, ale możliwa jest również klasyczna kompilacja bezpośrednio przez **g++**.

### Metoda 1: CMake (Zalecana)

Automatycznie zarządza zależnościami i procesem budowania.

1.  Upewnij się, że jesteś w głównym katalogu projektu.
2.  Utwórz katalog na pliki budowania:  
    mkdir build  
    cd build  
    
3.  Wygeneruj pliki Makefile:  
    cmake ..  
    
4.  Skompiluj projekt:  
    make  
    
5.  Uruchom grę:  
    ./space_invaders  
    

### Metoda 2: Klasyczna kompilacja (g++)

Jeśli chcesz skompilować grę jedną komendą w terminalu:

1.  Będąc w folderze z plikami źródłowymi (.cpp i .h), wpisz:  
    g++ main.cpp Game.cpp Entities.cpp ConsoleUtils.cpp -o space_invaders -std=c++11 -pthread  
    Uwaga: Flaga -pthread jest konieczna do obsługi funkcji usypiania wątku (std::this_thread::sleep_for)._
2.  Uruchom grę:  
    ./space_invaders  
    

## Jak to działa: Silnik Gry

### Podstawowe założenie

Gra opiera się na **ciągłej pętli (Game Loop)**, która wykonuje się wiele razy na sekundę (ok. 20 klatek/s). Aby uniknąć irytującego migania terminala (flickering), zastosowano technikę "buforowania" – najpierw cały świat jest rysowany do zmiennej w pamięci (bufor tekstowy), a dopiero potem gotowa klatka jest wypisywana na ekran po uprzednim wyczyszczeniu konsoli.

### Cykl pojedynczej klatki

Pętla gry realizuje 5 kluczowych kroków w każdej iteracji:

1.  **Input (Wejście):**
    - Sprawdzenie, czy klawisz został wciśnięty, w sposób **nieblokujący** (gra nie zatrzymuje się, czekając na reakcję gracza).
2.  **Update (Aktualizacja):**
    - Przesunięcie jednostek (gracza, pocisków, obcych).
    - Wykrycie kolizji (czy pocisk trafił obcego?).
    - Logika formacji (zejście obcych w dół przy krawędzi).
    - Usunięcie martwych obiektów z pamięci.
3.  **Render (Rysowanie do pamięci):**
    - Stworzenie czystej "kartki" (wektora stringów).
    - Naniesienie symboli wszystkich aktywnych obiektów na tę kartkę.
4.  **Display (Wyświetlanie):**
    - Wyczyszczenie ekranu terminala.
    - Wypisanie gotowego bufora i wyniku.
5.  **Sleep (Uśpienie):**
    - Krótka pauza (np. 50ms), aby ustabilizować prędkość gry, by nie działała zbyt szybko na nowoczesnych procesorach.

## Architektura i Historia Projektu

Struktura projektu powstawała ewolucyjnie, zaczynając od fundamentów, a kończąc na punkcie wejścia. Poniżej przedstawiono "łańcuch zależności" – kolejność, w jakiej pliki były planowane i tworzone, co odzwierciedla architekturę systemu.

### Kolejność warstw (od dołu do góry):

1.  **Globals.h**
    - _Fundament._ Tutaj zdefiniowano stałe (wymiary ekranu) oraz podstawową strukturę matematyczną Vector2. Wszystkie inne pliki zależą od tego nagłówka.
2.  **Entities.h**
    - _Modele danych._ Definicja klas obiektów. Stworzono klasę bazową Entity oraz dziedziczące po niej: Player, Alien, Bullet.
3.  **Entities.cpp**
    - _Logika obiektów._ Implementacja zachowań zdefiniowanych w pliku nagłówkowym (jak obiekty się poruszają, jak rysują się do bufora, jak wykrywają kolizje).
4.  **ConsoleUtils.h**
    - _Interfejs systemowy._ Deklaracje funkcji specyficznych dla systemu operacyjnego (ukrywanie kursora, nieblokujące wejście).
5.  **ConsoleUtils.cpp**
    - _Implementacja systemowa._ "Brudna robota" niskopoziomowa. Tu znajduje się kod zależny od OS (Linux termios vs Windows conio.h), odizolowany od reszty gry.
6.  **Game.h**
    - _Plan silnika._ Definicja klasy zarządcy Game, która przechowuje wskaźniki na obiekty (Player\*, vector&lt;Alien\*&gt;) i definicje metod sterujących pętlą.
7.  **Game.cpp**
    - _Implementacja silnika._ "Mózg" gry. Tutaj łączą się wszystkie wcześniejsze elementy: obsługa wejścia (ConsoleUtils), zarządzanie obiektami (Entities) i logika pętli głównej.
8.  **main.cpp**
    - _Punkt wejścia._ Najwyższa warstwa. Plik jest minimalny – jego jedynym zadaniem jest utworzenie instancji Game i uruchomienie metody run().