#include "includes/PokerHand.hpp"

PokerHand::PokerHand(const std::string& name, int rank)
	: name(name), rank(rank), baseMult(), baseChips() {}

PokerHand::~PokerHand() {}

PokerHand::PokerHand() : name(""), rank(0), baseMult(), baseChips() {}

PokerHand &PokerHand::operator=(const PokerHand &other) {
	if (this != &other) {
		name = other.name;
		rank = other.rank;
		baseMult = other.baseMult;
		baseChips = other.baseChips;
	}
	return *this;
}

std::string PokerHand::getName() {
	return name;
}

int PokerHand::getRank() {
	return rank;
}

int PokerHand::getChips() {
	return baseChips;
}

int PokerHand::getMult() {
	return baseMult;
}

void PokerHand::setPokerHand(std::string name1, int rank1, int baseChips1, int baseMult1) {
    name = name1;
    rank = rank1;
	baseChips = baseChips1;
	baseMult = baseMult1;
}