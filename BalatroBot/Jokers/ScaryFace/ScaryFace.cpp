#include "ScaryFace.hpp"

ScaryFace::ScaryFace() :bot(), cost(4), effect("Played face cards give +30 Chips when scored"), name("Scary Face"), rarity("Common") {}

ScaryFace::~ScaryFace() {}

ScaryFace::ScaryFace(const ScaryFace &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void ScaryFace::printJoker(){
    // Implementazione UI se necessaria
}


void ScaryFace::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "J" || rank == "Q" || rank == "K") {
			chips += 30;
		}
	}
}

int ScaryFace::getCost(){
	return cost;
}

std::string ScaryFace::getName() {
	return name;
}

std::string ScaryFace::getEffect() {
	return effect;
}

std::string ScaryFace::getRarity() {
	return rarity;
}