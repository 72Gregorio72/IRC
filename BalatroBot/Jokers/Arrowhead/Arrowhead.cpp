#include "Arrowhead.hpp"

Arrowhead::Arrowhead() :bot(), cost(7), effect("Played cards with Spade suit icon Spade suit give +50 Chips when scored"), name("Arrowhead"), rarity("Uncommon") {}

Arrowhead::~Arrowhead() {}

Arrowhead::Arrowhead(const Arrowhead &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Arrowhead::printJoker(){
    // Implementazione UI se necessaria
}


void Arrowhead::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string suit = selectedCards[i].getSuit();
		if (suit == "Spades") {
			chips += 50;
		}
	}
}

int Arrowhead::getCost(){
	return cost;
}

std::string Arrowhead::getName() {
	return name;
}

std::string Arrowhead::getEffect() {
	return effect;
}

std::string Arrowhead::getRarity() {
	return rarity;
}