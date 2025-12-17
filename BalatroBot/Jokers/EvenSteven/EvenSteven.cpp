#include "EvenSteven.hpp"

EvenSteven::EvenSteven() :bot(), cost(4), effect("Played cards with even rank give +4 Mult when scored"), name("Even Steven"), rarity("Common") {}

EvenSteven::~EvenSteven() {}

EvenSteven::EvenSteven(const EvenSteven &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void EvenSteven::printJoker(){
    // Implementazione UI se necessaria
}


void EvenSteven::playJoker(int& chips, int& mult, Balatro *bot){
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

		if (rankValue % 2 == 0) {
			mult += 4;
		}
	}
}

int EvenSteven::getCost(){
	return cost;
}

std::string EvenSteven::getName() {
	return name;
}

std::string EvenSteven::getEffect() {
	return effect;
}

std::string EvenSteven::getRarity() {
	return rarity;
}