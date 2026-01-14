#ifndef GLOBALS_H
#define GLOBALS_H

/**
 * @file Globals.h
 *
 * @brief Definicje stałych globalnych oraz podstawowych struktur danych.
 *
 * Plik ten zawiera elementy współdzielone przez cały projekt, takie jak
 * wymiary ekranu gry oraz strukturę wektora 2D używaną do określania pozycji obiektów
 */

 

/**
 * @brief Szerokość ekranu gry (w znakach konsoli).
 *
 * Określa liczbę kolumn w buforze i na ekranie.
 */
const int SCREEN_WIDTH = 40;

/**
 * @brief Wysokość ekranu gry (w wierszach konsoli).
 *
 * Określa liczbę wierszy w buforze i na ekranie.
 */
const int SCREEN_HEIGHT = 20;

/**
 * @brief Prosta struktura reprezentująca wektor dwuwymiarowy lub punkt.
 *
 * Używana do przechowywania pozycji obiektów (Gracza, Obcych, Pocisków)
 * w przestrzeni gry. Używa typu float dla płynności ruchu, choć przy renderowaniu
 * wartości są rzutowane na int.
 */
struct Vector2 {
    /**
     * @brief Współrzędna pozioma (X).
     */
    float x;

    /**
     * @brief Współrzędna pionowa (Y).
     */
    float y;

    /**
     * @brief Konstruktor struktury Vector2.
     *
     * Pozwala na szybką inicjalizację wektora.
     *
     * @param _x Wartość początkowa dla współrzędnej X (domyślnie 0).
     * @param _y Wartość początkowa dla współrzędnej Y (domyślnie 0).
     */
    Vector2(float _x = 0, float _y = 0) : x(_x), y(_y) {}
};

#endif // GLOBALS_H  