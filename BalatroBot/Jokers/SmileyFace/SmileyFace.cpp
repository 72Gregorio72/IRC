#include "SmileyFace.hpp"

SmileyFace::SmileyFace() :bot(), cost(4), effect("Played face cards give +5 Mult when scored"), name("Smiley Face"), rarity("Uncommon") {}

SmileyFace::~SmileyFace() {}

SmileyFace::SmileyFace(const SmileyFace &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void SmileyFace::printJoker(){
    // Implementazione UI se necessaria
}


void SmileyFace::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "J" || rank == "Q" || rank == "K") {
			mult += 5;
		}
	}
}

int SmileyFace::getCost(){
	return cost;
}

std::string SmileyFace::getName() {
	return name;
}

std::string SmileyFace::getEffect() {
	return effect;
}

std::string SmileyFace::getRarity() {
	return rarity;
}