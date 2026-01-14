#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

 /** 
 * @brief Deklaracje funkcji narzędziowych do obsługi konsoli systemowej.
 *
 * Plik zawiera interfejs dla funkcji systemowych (zależnych od OS), które pozwalają na:
 * - Konfigurację konsoli (np. ukrywanie kursora).
 * - Nieblokującą obsługę wejścia klawiatury (kluczowe dla pętli gry).
 * - Czyszczenie ekranu.
 * Implementacje tych funkcji znajdują się w pliku ConsoleUtils.cpp i różnią się
 * w zależności od systemu operacyjnego (Windows vs Linux/macOS).
 */

/**
 * @file ConsoleUtils.h
 */


/**
 * @brief Konfiguruje początkowe ustawienia konsoli.
 *
 * Funkcja ta powinna być wywołana na początku działania programu.
 * Jej głównym zadaniem jest przygotowanie środowiska do wyświetlania gry,
 * np. poprzez ukrycie migającego kursora, co zapobiega irytującym efektom wizualnym
 * podczas częstego odświeżania ekranu.
 */
void setupConsole();

/**
 * @brief Sprawdza, czy został wciśnięty klawisz, nie zatrzymując programu.
 *
 * Jest to funkcja typu "non-blocking". W przeciwieństwie do standardowego std::cin,
 * ta funkcja natychmiast zwraca wynik.
 *
 * @return true Jeśli w buforze klawiatury znajduje się oczekujący znak.
 * @return false Jeśli bufor jest pusty.
 */
bool isKeyPressed();

/**
 * @brief Pobiera ostatni wciśnięty znak z bufora klawiatury.
 *
 * Funkcja ta powinna być wywoływana zazwyczaj po upewnieniu się przez isKeyPressed(),
 * że znak jest dostępny.
 *
 * @return char Znak odpowiadający wciśniętemu klawiszowi.
 */
char getKey();

/**
 * @brief Czyści zawartość ekranu konsoli.
 *
 * Usuwa wszystkie znaki z widocznego obszaru terminala i ustawia kursor
 * w lewym górnym rogu (pozycja 0,0). Używana w każdej klatce przed
 * narysowaniem nowej sceny gry.
 */
void clearScreen();

#endif // CONSOLE_UTILS_H


 