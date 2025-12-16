#include "OnixAgate.hpp"

OnixAgate::OnixAgate() :bot(), cost(7), effect("Played cards with Club suit icon Club suit give +7 Mult when scored"), name("Onix Agate"), rarity("Uncommon") {}

OnixAgate::~OnixAgate() {}

OnixAgate::OnixAgate(const OnixAgate &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void OnixAgate::printJoker(){
    // Implementazione UI se necessaria
}


void OnixAgate::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string suit = selectedCards[i].getSuit();
		if (suit == "Clubs") {
			mult += 7;
		}
	}
}

int OnixAgate::getCost(){
	return cost;
}

std::string OnixAgate::getName() {
	return name;
}

std::string OnixAgate::getEffect() {
	return effect;
}

std::string OnixAgate::getRarity() {
	return rarity;
}