#include "Card.h"

std::ostream& operator<<(std::ostream& os, const Card& card) {
    os << card.toString();
    return os;
}