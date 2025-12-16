#include "Photograph.hpp"

Photograph::Photograph() :bot(), cost(5), effect("First played face card gives X2 Mult when scored"), name("Photograph"), rarity("Common") {}

Photograph::~Photograph() {}

Photograph::Photograph(const Photograph &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Photograph::printJoker(){
    // Implementazione UI se necessaria
}


void Photograph::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	bool faceCardPlayed = false;
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "J" || rank == "Q" || rank == "K") {
			faceCardPlayed = true;
			break;
		}
	}
	if (faceCardPlayed) {
		mult *= 2;
	}
}

int Photograph::getCost(){
	return cost;
}

std::string Photograph::getName() {
	return name;
}

std::string Photograph::getEffect() {
	return effect;
}

std::string Photograph::getRarity() {
	return rarity;
}