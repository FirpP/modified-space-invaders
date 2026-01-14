#include "ConsoleUtils.h"
#include <iostream>
#include <cstdio>

/**
 * @file ConsoleUtils.cpp
 * 
 */

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

void setupConsole() 
{
    // Ukrycie kursora
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

bool isKeyPressed() 
{ 
    return _kbhit(); 
}

char getKey() 
{ 
    return _getch(); 
}

void clearScreen() # Rozpoczęcie pierwszej fazy gry
{ 
    system("cls"); 
}

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

void setupConsole() 
{
    // Ukrycie kursora kodem ANSI
    std::cout << "\033[?25l";
}

/**
 * @brief Sprawdza w sposób nieblokujący, czy naciśnięto klawisz klawiatury.
 *
 * Funkcja ta emuluje zachowanie znanej z systemu Windows funkcji _kbhit()
 * w środowiskach zgodnych z POSIX (Linux, macOS). Pozwala sprawdzić stan
 * bufora wejścia bez zatrzymywania działania programu.
 *
 * @details
 * Działanie funkcji opiera się na tymczasowej modyfikacji ustawień terminala:
 * 1. Pobiera obecne ustawienia terminala (struktura termios).
 * 2. Wyłącza tryb kanoniczny (ICANON), aby znaki były dostępne natychmiast
 * bez konieczności wciskania Enter.
 * 3. Wyłącza echo (ECHO), aby testowany znak nie pojawiał się na ekranie.
 * 4. Ustawia deskryptor wejścia (STDIN) w tryb nieblokujący (O_NONBLOCK),
 * dzięki czemu próba odczytu nie zatrzyma programu, jeśli bufor jest pusty.
 * 5. Próbuje pobrać jeden znak funkcją getchar().
 * 6. Natychmiast przywraca oryginalne ustawienia terminala.
 * 7. Jeśli znak został pobrany, jest zwracany do bufora (ungetc), aby
 * kolejna funkcja (np. getKey) mogła go poprawnie odczytać.
 *
 * @return int 
 * - 1 (true): Jeśli w buforze wejścia znajduje się znak (klawisz został wciśnięty).
 * - 0 (false): Jeśli bufor jest pusty (brak wciśniętego klawisza).
 *
 * @note Funkcja jest przeznaczona dla systemów uniksowych (Linux, macOS).
 * Wymaga bibliotek <termios.h>, <unistd.h> oraz <fcntl.h>.
 */
int kbhit(void) 
{
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    //1. Pobranie ustawień
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    //2. wyłączenie buforowania i echa
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    //3. ustawienie trybu "non-blocking"
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

bool isKeyPressed() 
{ 
    return kbhit(); 
}

char getKey() 
{ 
    return getchar();
}

void clearScreen() 
{ 
    //czyszczenie terminalu i reset cursora
    std::cout << "\033[2J\033[1;1H"; 
}
#endif