#include "BusinessCard.hpp"

BusinessCard::BusinessCard() :bot(), cost(4), effect("Played face cards have a 1 in 2 chance to give $2 when scored"), name("Business Card"), rarity("Common") {}

BusinessCard::~BusinessCard() {}

BusinessCard::BusinessCard(const BusinessCard &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void BusinessCard::printJoker(){
    // Implementazione UI se necessaria
}


void BusinessCard::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "J" || rank == "Q" || rank == "K") {
			int randomValue = rand() % 2; // Genera un numero casuale tra 0 e 1
			if (randomValue == 0) {
				chips += 2;
			}
		}
	}
}

int BusinessCard::getCost(){
	return cost;
}

std::string BusinessCard::getName() {
	return name;
}

std::string BusinessCard::getEffect() {
	return effect;
}

std::string BusinessCard::getRarity() {
	return rarity;
}