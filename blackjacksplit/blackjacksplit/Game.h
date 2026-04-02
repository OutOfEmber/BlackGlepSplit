#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "Actors.h"
#include <string>

class IFormattable {
public:
    virtual std::string format() const = 0;
    virtual ~IFormattable() {}
};

class DeckAdapter : public IFormattable {
    Deck& deck;
public:
    DeckAdapter(Deck& d) : deck(d) {}
    std::string format() const override {
        return "В колоде осталось: " + std::to_string(deck.getSize()) + " карт";
    }
};

class Game {
private:
    Deck deck;
    Player hands[2];
    Dealer dealer;
    int balance = 10000;
    int currentBet = 0;
    bool isSplit = false;
    void reset();
    int getSafeInt();
    void prettyPrint(const IFormattable& object);

public:
    void play();
};

#endif