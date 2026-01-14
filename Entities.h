#ifndef ENTITIES_H
#define ENTITIES_H

#include "Globals.h"
#include <vector>
#include <string>
#include <cmath>

/**
 * @file Entities.h
 * @brief Definicje klas obiektów gry (Gracz, Obcy, Pociski).
 */

/**
 * @brief abstrakcyjna klasa bazowa reprezentująca każdy obiekt w grze.
 * 
 * Klasa Entity definiuje wspólny interfejs i właściwości dla wszystkich bytów
 * występujących na planszy (Gracza, Obcych, Pocisków). Zawiera informacje
 * o pozycji, wyglądzie (symbolu) oraz stanie aktywności.
 */
class Entity {
public:
    /**
     * @brief Pozycja obiektu na ekranie (współrzędne x, y).
     */
    Vector2 pos;

    /**
     * @brief Znak ASCII reprezentujący obiekt na konsoli (np. 'A', 'M', '|').
     */
    char symbol;

    /**
     * @brief Flaga określająca, czy obiekt jest "żywy".
     * * Jeśli active == false, obiekt zostanie usunięty z pamięci przez silnik gry
     * w najbliższej fazie czyszczenia.
     */
    bool active;

    /**
     * @brief Konstruktor klasy Entity.
     * @param x Początkowa współrzędna X.
     * @param y Początkowa współrzędna Y.
     * @param s Znak (symbol) reprezentujący obiekt.
     */
    Entity(float x, float y, char s);

    /**
     * @brief Wirtualny destruktor.
     * * Konieczny dla bezpiecznego usuwania obiektów przez wskaźnik na klasę bazową.
     */
    virtual ~Entity();

    /**
     * @brief Czysto wirtualna metoda aktualizacji stanu obiektu.
     * * Każda klasa pochodna musi zaimplementować własną logikę ruchu/zachowania.
     */
    virtual void update() = 0; 

    /**
     * @brief Rysuje obiekt na wirtualnym buforze ekranu.
     * * Metoda sprawdza, czy obiekt znajduje się w granicach ekranu, a następnie
     * wstawia jego symbol w odpowiednie miejsce w dwuwymiarowej tablicy znaków.
     * @param buffer Referencja do bufora ekranu (wektor stringów).
     */
    void draw(std::vector<std::string>& buffer);

    /**
     * @brief Sprawdza kolizję z innym obiektem typu Entity.
     * * Wykorzystuje proste sprawdzanie odległości (AABB - Axis-Aligned Bounding Box)
     * o wymiarach 1x1 jednostka.
     * @param other Referencja do drugiego obiektu, z którym sprawdzamy kolizję.
     * @return true Jeśli obiekty nachodzą na siebie.
     * @return false Jeśli brak kolizji lub któryś z obiektów jest nieaktywny.
     */
    bool checkCollision(const Entity& other);
};

/**
 * @brief Klasa reprezentująca pocisk.
 * * Dziedziczy po klasie Entity. Może poruszać się w górę lub w dół (zależnie od prędkości).
 */
class Bullet : public Entity {
public:
    /**
     * @brief Prędkość pionowa pocisku.
     * * Wartość ujemna oznacza ruch w górę (strzał gracza).
     * * Wartość dodatnia oznacza ruch w dół (strzał obcego).
     */
    float speed;

    /**
     * @brief Konstruktor pocisku.
     * @param x Pozycja startowa X.
     * @param y Pozycja startowa Y.
     * @param s Prędkość (speed) - określa kierunek i szybkość lotu.
     */
    Bullet(float x, float y, float s);

    /**
     * @brief Aktualizuje pozycję pocisku.
     * * Przesuwa pocisk w pionie. Jeśli wyleci poza ekran, ustawia flagę active na false.
     */
    void update() override;
};

/**
 * @brief Klasa reprezentująca statek gracza.
 * * Sterowana przez użytkownika, porusza się tylko w poziomie na dole ekranu.
 */
class Player : public Entity {
public:
    /**
     * @brief Konstruktor gracza.
     * * Ustawia gracza w domyślnej pozycji startowej (dół ekranu, środek).
     */
    Player();

    /**
     * @brief Aktualizuje stan gracza.
     * * Głównie odpowiada za pilnowanie granic ekranu (clamping), aby gracz nie wyjechał poza planszę.
     */
    void update() override;

    /**
     * @brief Przesuwa gracza o zadaną wartość.
     * @param dir Wartość przesunięcia (np. -2.0f dla lewo, 2.0f dla prawo).
     */
    void move(float dir);
};

/**
 * @brief Klasa reprezentująca przeciwnika (Obcego).
 * * Porusza się w formacji: poziomo, a po dotarciu do krawędzi schodzi w dół.
 */
class Alien : public Entity {
public:
    /**
     * @brief Prędkość poruszania się w poziomie.
     */
    float speed;

    /**
     * @brief Kierunek ruchu poziomego.
     * * 1: Ruch w prawo.
     * * -1: Ruch w lewo.
     */
    int direction; 

    /**
     * @brief Konstruktor Obcego.
     * @param x Pozycja startowa X.
     * @param y Pozycja startowa Y.
     */
    Alien(float x, float y);

    /**
     * @brief Aktualizuje pozycję obcego.
     * * Przesuwa obiekt w poziomie zgodnie z aktualną prędkością i kierunkiem.
     */
    void update() override;

    /**
     * @brief Powoduje zejście obcego o jeden poziom w dół i zmianę kierunku ruchu.
     * * Wywoływana przez zarządcę gry (Game), gdy formacja dotrze do krawędzi ekranu.
     */
    void dropDown();
};

#endif // ENTITIES_H