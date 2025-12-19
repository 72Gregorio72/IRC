#include "includes/PokerHand.hpp"

PokerHand::PokerHand(const std::string& name, int rank)
	: name(name), rank(rank), level(0), baseMult(), baseChips() {}

PokerHand::~PokerHand() {}

PokerHand::PokerHand() : name(""), rank(0), level(0), baseMult(), baseChips() {}
PokerHand &PokerHand::operator=(const PokerHand &other) {
	if (this != &other) {
		name = other.name;
		rank = other.rank;
		baseMult = other.baseMult;
		baseChips = other.baseChips;
		level = other.level;
	}
	return *this;
}

void PokerHand::setChips(int chips) {
	baseChips = chips;
}

void PokerHand::setMult(int mult) {
	baseMult = mult;
}

void PokerHand::setLevel(int level1) {
	level = level1;
}

std::string PokerHand::getName() {
	return name;
}

int PokerHand::getRank() {
	return rank;
}

int PokerHand::getLevel() {
	return level;
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