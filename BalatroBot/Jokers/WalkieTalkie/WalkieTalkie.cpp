#include "WalkieTalkie.hpp"

WalkieTalkie::WalkieTalkie() :bot(), cost(4), effect("Each played 10 or 4 gives +10 Chips and +4 Mult when scored"), name("WalkieTalkie"), rarity("Common") {}

WalkieTalkie::~WalkieTalkie() {}

WalkieTalkie::WalkieTalkie(const WalkieTalkie &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void WalkieTalkie::printJoker(){
    // Implementazione UI se necessaria
}


void WalkieTalkie::playJoker(int& chips, int& mult, Balatro *bot){
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "10" || rank == "4") {
			chips += 10;
			mult += 4;
		}
	}
}

int WalkieTalkie::getCost(){
	return cost;
}

std::string WalkieTalkie::getName() {
	return name;
}

std::string WalkieTalkie::getEffect() {
	return effect;
}

std::string WalkieTalkie::getRarity() {
	return rarity;
}