#include "Deck.h"
#include <algorithm>
#include <random>

using namespace std;

void Deck::generate() {
    char hearts = 3, bubbles = 4, treph = 5, romb = 6;
    cards.clear();
    std::string suits[] = { string(1,hearts), string(1,bubbles), string(1,treph), string(1,romb) };
    std::string ranks[] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
    int values[] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11 };

    for (int d = 0; d < 4; d++) {
        for (int s = 0; s < 4; s++) {
            for (int r = 0; r < 13; r++) {
                cards.push_back(new Card(ranks[r], suits[s], values[r]));
            }
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
}

Card* Deck::drawCard() {
    if (cards.empty()) return nullptr;
    Card* c = cards.back();
    cards.pop_back();
    return c;
}

int Deck::getSize() const {
    return static_cast<int>(cards.size());
}