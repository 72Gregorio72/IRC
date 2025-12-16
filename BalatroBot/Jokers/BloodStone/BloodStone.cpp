#include "BloodStone.hpp"

BloodStone::BloodStone() :bot(), cost(7), effect("1 in 2 chance for played cards with Heart suit icon Heart suit to give X1.5 Mult when scored"), name("Blood Stone"), rarity("Uncommon") {}

BloodStone::~BloodStone() {}

BloodStone::BloodStone(const BloodStone &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void BloodStone::printJoker(){
    // Implementazione UI se necessaria
}


void BloodStone::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string suit = selectedCards[i].getSuit();
		if (suit == "Hearts") {
			int randomValue = rand() % 2; // Genera un numero casuale tra 0 e 1
			if (randomValue == 0) {
				mult = static_cast<int>(mult * 1.5);
			}
		}
	}
}

int BloodStone::getCost(){
	return cost;
}

std::string BloodStone::getName() {
	return name;
}

std::string BloodStone::getEffect() {
	return effect;
}

std::string BloodStone::getRarity() {
	return rarity;
}