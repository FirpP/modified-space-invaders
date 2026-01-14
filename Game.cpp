/**
 * @file Game.cpp
 */
 
#include "Game.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

/**
 * @brief Implementacja logiki silnika gry.
 *
 * Plik ten zawiera definicje metod klasy Game, które odpowiadają za:
 * - Inicjalizację zasobów (konstruktor).
 * - Zarządzanie cyklem życia obiektów (destruktor).
 * - Obsługę wejścia (klawiatura).
 * - Logikę matematyczną i fizykę gry (update).
 * - Rysowanie stanu gry na ekranie (render).
 * - Główną pętlę sterującą czasem gry (run).
  */

/**
 * @brief Konstruktor klasy Game.
 *
 * Inicjalizuje stan początkowy gry:
 * 1. Ustawia flagę isRunning na true.
 * 2. Zeruje wynik (score).
 * 3. Konfiguruje konsolę (ukrywa kursor).
 * 4. Tworzy obiekt gracza na stercie.
 * 5. Inicjalizuje formację obcych.
 */
Game::Game() : isRunning(true), score(0), shiftDown(false) 
{
    setupConsole();
    player = new Player();
    initAliens();
}

/**
 * @brief Destruktor klasy Game.
 *
 * Sprząta po zakończeniu gry, zwalniając pamięć zaalokowaną dynamicznie.
 * Usuwa gracza, wszystkich pozostałych obcych oraz wszystkie aktywne pociski.
 * Na końcu przywraca widoczność kursora w terminalu.
 */
Game::~Game() 
{
    delete player;
    for (auto a : aliens) 
    {
        delete a;
    }
    
    for (auto b : bullets) 
    {
        delete b;
    }
    // Przywrócenie kursora
    std::cout << "\033[?25h"; 
}

/**
 * @brief Tworzy początkową formację obcych.
 *
 * Generuje siatkę przeciwników (4 rzędy po 8 kolumn).
 * Obcy są rozmieszczani w stałych odstępach, zaczynając od pozycji (5, 2).
 * Nowe obiekty są dodawane do wektora `aliens`.
 */
void Game::initAliens() 
{
    for (int y = 0; y < 4; y++) 
    {
        for (int x = 0; x < 8; x++) 
        {
            aliens.push_back(new Alien(5 + (x * 3), 2 + (y * 2)));
        }
    }
}


 /**
 * @brief Obsługuje sterowanie grą.
 *
 * Sprawdza w sposób nieblokujący, czy użytkownik wcisnął klawisz.
 * - 'q': Kończy grę (isRunning = false).
 * - 'a': Przesuwa gracza w lewo.
 * - 'd': Przesuwa gracza w prawo.
 * - ' ': Tworzy nowy pocisk na pozycji gracza, lecący w górę.
 */

void Game::handleInput() 
{
    if (isKeyPressed()) 
    {
        char k = getKey();
        if (k == 'q') isRunning = false;
        if (k == 'a') player->move(-2.0f);
        if (k == 'd') player->move(2.0f);
        if (k == ' ') 
        {
            bullets.push_back(new Bullet(player->pos.x, player->pos.y - 1, -1.0f));
        }
    }
}


 /**
 * @brief Główna funkcja aktualizująca stan gry (fizykę).
 *
 * Wykonuje się w każdej klatce i realizuje następujące kroki:
 * 1. Aktualizuje pozycję gracza i wszystkich pocisków.
 * 2. Aktualizuje pozycję obcych i wykrywa, czy dotarli do krawędzi ekranu.
 * Jeśli tak, ustawia flagę `shiftDown`.
 * 3. Jeśli `shiftDown` jest true, obniża całą formację obcych.
 * 4. Sprawdza kolizje pocisków z obcymi. Trafienie usuwa oba obiekty i dodaje punkty.
 * 5. Usuwa nieaktywne (zniszczone lub wyleciałe poza ekran) obiekty z pamięci (std::remove_if + delete).
 * 6. Sprawdza warunki końca gry (brak obcych -> respawn, obcy na dole -> Game Over).
 */
void Game::update() 
{
    player->update();

    // 1. Aktualizacja pocisków
    for (auto b : bullets) b->update();

    // 2. Aktualizacja obcych
    shiftDown = false;
    for (auto a : aliens) 
    {
        if (!a->active) continue;
        a->update();
        if ((a->pos.x <= 1 && a->direction == -1) || 
            (a->pos.x >= SCREEN_WIDTH - 2 && a->direction == 1)) 
            {
            shiftDown = true;
        }
    }

    if (shiftDown) 
    {
        for (auto a : aliens) a->dropDown();
    }

    // 3. Kolizje
    for (auto b : bullets) {
        if (!b->active) continue;
        for (auto a : aliens) {
            if (a->active && b->checkCollision(*a)) {
                a->active = false;
                b->active = false;
                score += 10;
                break;
            }
        }
    }

    // 4. Czyszczenie nieaktywnych obiektów
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [](Bullet* b) { 
            if(!b->active) { delete b; return true; }
            return false; 
        }), bullets.end());

    aliens.erase(std::remove_if(aliens.begin(), aliens.end(), 
        [](Alien* a) { 
            if(!a->active) { delete a; return true; }
            return false; 
        }), aliens.end());

    // Sprawdzenie warunków końca gry
    if (aliens.empty()) {
        initAliens(); 
    }
    for (auto a : aliens) {
        if (a->pos.y >= SCREEN_HEIGHT - 2) {
            isRunning = false;
        }
    }
}

/**
 * @brief Rysuje klatkę gry.
 *
 * 1. Tworzy pusty bufor znakowy (vector stringów).
 * 2. Prosi wszystkie obiekty (gracza, obcych, pociski) o narysowanie się na buforze.
 * 3. Czyści ekran konsoli.
 * 4. Wyświetla interfejs (wynik, ramki) oraz zawartość bufora.
 */
void Game::render() {
    std::vector<std::string> buffer(SCREEN_HEIGHT, std::string(SCREEN_WIDTH, ' '));

    player->draw(buffer);
    for (auto a : aliens) a->draw(buffer);
    for (auto b : bullets) b->draw(buffer);

    clearScreen();

    std::cout << "SCORE: " << score << "\n";
    std::string border(SCREEN_WIDTH + 2, '#');
    std::cout << border << "\n";
    
    for (const auto& row : buffer) {
        std::cout << "#" << row << "#\n";
    }
    
    std::cout << border << "\n";
}

/**
 * @brief Główna pętla gry.
 *
 * Utrzymuje działanie gry dopóki isRunning jest true.
 * W każdej iteracji:
 * 1. Pobiera wejście (handleInput).
 * 2. Aktualizuje stan świata (update).
 * 3. Rysuje świat (render).
 * 4. Usypia wątek na 50ms, aby uzyskać stałe ~20 klatek na sekundę (FPS).
 */
void Game::run() {
    while (isRunning) {
        handleInput();
        update();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "GAME OVER! Final Score: " << score << std::endl;
}