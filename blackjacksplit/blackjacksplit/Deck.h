#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card*> cards;
public:
    void generate();
    Card* drawCard();
    int getSize() const; 
};

#endif