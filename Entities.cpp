#include "Entities.h"


/**
 * @file Entities.cpp
 * 
 *
 * @brief Implementacja metod klas zdefiniowanych w Entities.h.
 *
 * Plik ten zawiera kod wykonawczy dla logiki obiektów gry.
 * Znajdują się tutaj implementacje metod odpowiedzialnych za:
 * - Rysowanie obiektów do bufora (Entity::draw).
 * - Wykrywanie kolizji (Entity::checkCollision).
 * - Aktualizację pozycji i stanu (update) dla Gracza, Obcych i Pocisków. 
 */




/**
 * @brief Konstruktor klasy bazowej Entity.
 * * Inicjalizuje podstawowe parametry wspólne dla wszystkich obiektów.
 * Używa listy inicjalizacyjnej dla optymalizacji.
 * * @param x Współrzędna początkowa X.
 * @param y Współrzędna początkowa Y.
 * @param s Znak (symbol), który będzie reprezentował obiekt.
 */
Entity::Entity(float x, float y, char s) : pos(x, y), symbol(s), active(true) {}

/**
 * @brief Wirtualny destruktor.
 * * Puste ciało, ale niezbędne dla poprawności polimorfizmu w C++.
 */
Entity::~Entity() {}

/**
 * @brief Wstawia symbol obiektu do bufora ekranu.
 * * Metoda sprawdza, czy obiekt znajduje się fizycznie w granicach ekranu
 * (0 <= x < SCREEN_WIDTH oraz 0 <= y < SCREEN_HEIGHT).
 * Jeśli tak, wpisuje znak `symbol` pod odpowiednie indeksy tablicy `buffer`.
 * * @param buffer Referencja do wektora stringów reprezentującego klatkę obrazu.
 */
void Entity::draw(std::vector<std::string>& buffer) 
{
    if (!active) return;
    int px = static_cast<int>(pos.x);
    int py = static_cast<int>(pos.y);

    if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) 
    {
        buffer[py][px] = symbol;
    }
}

/**
 * @brief Sprawdza kolizję metodą prostokątów (AABB) o wielkości 1x1.
 * * Oblicza różnicę odległości w osi X i Y po zrzutowaniu na liczby całkowite.
 * Jeśli oba obiekty zajmują tę samą "kratkę" w terminalu, następuje kolizja.
 * * @param other Obiekt, z którym sprawdzamy kolizję.
 * @return true Jeśli odległość < 1 w obu osiach.
 */
bool Entity::checkCollision(const Entity &other) 
{
    if (!active || !other.active)
    {
        return false;
    } 
    int dx = static_cast<int>(pos.x) - static_cast<int>(other.pos.x);
    int dy = static_cast<int>(pos.y) - static_cast<int>(other.pos.y);
    return (std::abs(dx) < 1 && std::abs(dy) < 1);
}

/**
 * @brief Konstruktor pocisku.
 * * Ustawia symbol na '|' (pionowa kreska) i przypisuje prędkość.
 * * @param x Pozycja startowa X.
 * @param y Pozycja startowa Y.
 * @param s Prędkość (ujemna leci w górę, dodatnia w dół).
 */
Bullet::Bullet(float x, float y, float s) : Entity(x, y, '|'), speed(s) {}

/**
 * @brief Logika ruchu pocisku.
 * * Przesuwa pocisk w pionie o wartość `speed`.
 * Sprawdza, czy pocisk wyleciał poza ekran (góra/dół). Jeśli tak,
 * dezaktywuje go (active = false), co pozwoli na usunięcie go z pamięci w Game::update.
 */
void Bullet::update() 
{
    pos.y += speed;
    if (pos.y < 0 || pos.y >= SCREEN_HEIGHT) 
    {
        active = false;
    }
}

/**
 * @brief Konstruktor gracza.
 * * Ustawia gracza na środku dolnej części ekranu z symbolem 'A'.
 * SCREEN_WIDTH / 2 to środek osi X.
 * SCREEN_HEIGHT - 2 to bezpieczna odległość od dolnej krawędzi.
 */
Player::Player() : Entity(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 2.0f, 'A') {}

/**
 * @brief Aktualizacja stanu gracza - ograniczenie ruchu (Clamping).
 * * Gracz nie porusza się tutaj sam z siebie (ruch sterowany jest w handleInput),
 * ale ta metoda upewnia się, że gracz nie wyjedzie poza lewą (x < 1)
 * lub prawą (x > WIDTH - 2) krawędź ekranu.
 */
void Player::update() 
{
    // Ograniczenie ruchu do ekranu
    if (pos.x < 1) pos.x = 1;
    if (pos.x > SCREEN_WIDTH - 2) pos.x = SCREEN_WIDTH - 2;
}

/**
 * @brief Przesuwa gracza w poziomie.
 * * @param dir Wartość przesunięcia (dodatnia w prawo, ujemna w lewo).
 */
void Player::move(float dir) 
{
    pos.x += dir;
}

/**
 * @brief Konstruktor obcego.
 * * Ustawia symbol 'M', domyślną prędkość 0.1f i początkowy kierunek w prawo (1).
 */
Alien::Alien(float x, float y) : Entity(x, y, 'M'), speed(0.1f), direction(1) {}

/**
 * @brief Logika automatycznego ruchu obcego.
 * * Przesuwa obcego w poziomie: x = x + (prędkość * kierunek).
 */
void Alien::update() 
{
    pos.x += speed * direction;
}

/**
 * @brief Manewr zejścia w dół.
 * * Wykonywany, gdy formacja dotrze do ściany.
 * 1. Zwiększa Y o 1.0 (zejście niżej).
 * 2. Odwraca kierunek (mnożenie przez -1), aby obcy zaczęli wracać w drugą stronę.
 */
void Alien::dropDown() 
{
    pos.y += 1.0f;
    direction *= -1; 
}
