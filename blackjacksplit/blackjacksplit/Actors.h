#ifndef ACTORS_H
#define ACTORS_H
#include <vector>
#include <stdexcept>
#include "Card.h"

class BustException : public std::runtime_error {
public:
    BustException() : std::runtime_error("ПЕРЕБОР!") {}
};

class Actor {   
protected:
    std::vector<Card*> hand;
    int score = 0;
public:
    virtual void addCard(Card* card) {
        if (!card) return;
        hand.push_back(card);
        score += card->getValue();
        if (score > 21) {
            for (auto c : hand) {
                if (c->getRank() == "A" && score > 21) score -= 10;
            }
        }
        if (score > 21) throw BustException();
    }
    void removeLastCard() {
        if (!hand.empty()) {
            score -= hand.back()->getValue();
            hand.pop_back();
        }
    }
    int getScore() const { return score; }
    const std::vector<Card*>& getHand() const { return hand; }
    void clear() { hand.clear(); score = 0; }
};

class Player : public Actor {};
class Dealer : public Actor {};
#endif