#include <iostream>
#include <windows.h>
#include <clocale>
#include "Game.h"

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");

    Game blackjack;
    blackjack.play();

    return 0;
}