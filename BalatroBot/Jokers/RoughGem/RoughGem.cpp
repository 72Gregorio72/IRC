#include "RoughGem.hpp"

RoughGem::RoughGem() :bot(), cost(6), effect("Played cards with Diamond suit earn $1 when scored"), name("Rough Gem"), rarity("Uncommon") {}

RoughGem::~RoughGem() {}

RoughGem::RoughGem(const RoughGem &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void RoughGem::printJoker(){
    // Implementazione UI se necessaria
}


void RoughGem::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string suit = selectedCards[i].getSuit();
		if (suit == "Diamonds") {
			bot->setCoins(bot->getCoins() + 1);
		}
	}
}

int RoughGem::getCost(){
	return cost;
}

std::string RoughGem::getName() {
	return name;
}

std::string RoughGem::getEffect() {
	return effect;
}

std::string RoughGem::getRarity() {
	return rarity;
}