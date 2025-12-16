#include "Scholar.hpp"

Scholar::Scholar() :bot(), cost(4), effect("Played Aces give +20 Chips and +4 Mult when scored"), name("Scholar"), rarity("Common") {}

Scholar::~Scholar() {}

Scholar::Scholar(const Scholar &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Scholar::printJoker(){
    // Implementazione UI se necessaria
}


void Scholar::playJoker(int& chips, int& mult, Balatro *bot){
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "A") {
			chips += 20;
			mult += 4;
		}
	}
}

int Scholar::getCost(){
	return cost;
}

std::string Scholar::getName() {
	return name;
}

std::string Scholar::getEffect() {
	return effect;
}

std::string Scholar::getRarity() {
	return rarity;
}