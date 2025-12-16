#include "OddTodd.hpp"

OddTodd::OddTodd() :bot(), cost(4), effect("Played cards with odd rank give +31 Chips when scored"), name("Odd Todd"), rarity("Common") {}

OddTodd::~OddTodd() {}

OddTodd::OddTodd(const OddTodd &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void OddTodd::printJoker(){
    // Implementazione UI se necessaria
}


void OddTodd::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		int rankValue = 0;
		if (rank == "J") rankValue = 11;
		else if (rank == "Q") rankValue = 12;
		else if (rank == "K") rankValue = 13;
		else if (rank == "A") rankValue = 14;
		else rankValue = std::atol(rank.c_str());

		if (rankValue % 2 != 0) {
			chips += 31;
		}
	}
}

int OddTodd::getCost(){
	return cost;
}

std::string OddTodd::getName() {
	return name;
}

std::string OddTodd::getEffect() {
	return effect;
}

std::string OddTodd::getRarity() {
	return rarity;
}