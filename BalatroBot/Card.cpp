#include "includes/Card.hpp"

Card::Card() : suit(""), rank("") {}

Card::Card(const std::string &suit, const std::string &rank) : suit(suit), rank(rank) {}

Card::~Card() {}

Card::Card(const Card &other) : suit(other.suit), rank(other.rank) {}

Card &Card::operator=(const Card &other) {
	if (this != &other) {
		suit = other.suit;
		rank = other.rank;
	}
	return *this;
}

std::string Card::getSuit() const {
	return suit;
}

std::string Card::getRank() const {
	return rank;
}