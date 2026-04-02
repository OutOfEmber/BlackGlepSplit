#ifndef CARD_H
#define CARD_H

#include <string>
#include <iostream>

class Card {
protected:
    std::string rank, suit;
    int value;

public:
    Card(std::string r, std::string s, int v) : rank(r), suit(s), value(v) {}
    virtual ~Card() {}

    int getValue() const { return value; }
    std::string getRank() const { return rank; }
    std::string getSuit() const { return suit; }

    virtual std::string toString() const {
        return rank + suit;
    }

    friend std::ostream& operator<<(std::ostream& os, const Card& card);
};

class PrettyCard : public Card {
public:
    using Card::Card;

    std::string toString() const override {
        return "[" + rank + " " + suit + "]";
    }
};

#endif