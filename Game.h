#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Entities.h"

/**
 * @file Game.h
 * @brief Definicja głównej klasy gry.
 */

/**
 * @brief Główna klasa silnika gry "Space Invaders".
 *
 * Klasa Game działa jako zarządca (manager) całej aplikacji. Jest odpowiedzialna za:
 * - Uruchomienie i utrzymanie głównej pętli gry.
 * - Przechowywanie stanu gry (punkty, czy gra działa).
 * - Zarządzanie cyklem życia wszystkich obiektów (Gracz, Obcy, Pociski).
 * - Koordynowanie logiki (aktualizacje, kolizje) oraz wyświetlania (renderowanie).
 */

class Game
{
private:
     /**
     * @brief Flaga kontrolująca działanie głównej pętli gry.
     * * Jeśli ustawiona na false (np. po wciśnięciu 'q' lub przegranej),
     * metoda run() zakończy działanie.
     */
    bool isRunning;

    /**
     * @brief Wskaźnik na obiekt gracza.
     * * Obiekt jest tworzony dynamicznie w konstruktorze i usuwany w destruktorze.
     */
    Player *player;

    /**
     * @brief Kontener przechowujący wskaźniki na wszystkich żywych obcych.
     * * Użycie std::vector pozwala na dynamiczne usuwanie zestrzelonych przeciwników.
     */
    std::vector<Alien*> aliens;

     /**
     * @brief Kontener przechowujący wskaźniki na wszystkie aktywne pociski.
     * * Zawiera zarówno pociski wystrzelone przez gracza, jak i (opcjonalnie) przez obcych.
     */
    std::vector<Bullet*> bullets;

     /**
     * @brief Aktualny wynik punktowy gracza.
     */
    int score;

    /**
     * @brief Flaga pomocnicza dla logiki poruszania się obcych.
     * * Ustawiana na true w metodzie update(), jeśli którykolwiek z obcych
     * dotknie bocznej krawędzi ekranu. Powoduje to przesunięcie całej grupy w dół.
     */
    bool shiftDown; // Flaga dla logiki ruchu obcych

    /**
     * @brief Inicjalizuje lub resetuje formację obcych na planszy.
     * * Tworzy siatkę obiektów Alien i dodaje je do wektora aliens.
     * Wywoływana w konstruktorze oraz po wyczyszczeniu poziomu.
     */
    void initAliens();

    /**
     * @brief Obsługuje wejście od użytkownika w sposób nieblokujący.
     * * Sprawdza stan klawiatury i podejmuje akcje:
     * - 'a'/'d': Poruszanie graczem.
     * - ' ' (spacja): Wystrzelenie pocisku.
     * - 'q': Wyjście z gry.
     */
    void handleInput();

    /**
     * @brief Aktualizuje logikę gry (fizykę i zasady) dla jednej klatki.
     * * Metoda ta wykonuje szereg zadań:
     * 1. Aktualizuje pozycje gracza, pocisków i obcych.
     * 2. Zarządza logiką "zejścia w dół" obcych przy krawędziach.
     * 3. Wykrywa i obsługuje kolizje (pocisk vs obcy).
     * 4. Usuwa nieaktywne obiekty z pamięci.
     * 5. Sprawdza warunki końca gry (Game Over).
     */
    void update();

    /**
     * @brief Rysuje aktualny stan gry na konsoli.
     * * Tworzy bufor znakowy, wypełnia go symbolami obiektów, czyści ekran
     * i wyświetla gotową klatkę wraz z wynikiem i ramką.
     */
    void render();

public:
    Game();
    ~Game();

     /**
     * @brief Uruchamia główną pętlę gry.
     * * Metoda ta sekwencyjnie wywołuje handleInput(), update() i render()
     * w nieskończonej pętli (dopóki isRunning jest true), kontrolując
     * jednocześnie szybkość działania gry (sleep).
     */
    void run();
};

#endif // GAME_H