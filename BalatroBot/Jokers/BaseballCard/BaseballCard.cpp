#include "BaseballCard.hpp"

BaseballCard::BaseballCard() :bot(), cost(8), effect("Uncommon Jokers each give X1.5 Mult"), name("Baseball Card"), rarity("Rare") {}

BaseballCard::~BaseballCard() {}

BaseballCard::BaseballCard(const BaseballCard &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void BaseballCard::printJoker(){
    // Implementazione UI se necessaria
}


void BaseballCard::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	for (size_t i = 0; i < bot->getJokers().size(); ++i) {
		IJoker* joker = bot->getJokers()[i];
		std::string rarity = joker->getRarity();
		if (rarity == "Uncommon") {
			mult *= 1.5;
		}
	}
}

int BaseballCard::getCost(){
	return cost;
}

std::string BaseballCard::getName() {
	return name;
}

std::string BaseballCard::getEffect() {
	return effect;
}

std::string BaseballCard::getRarity() {
	return rarity;
}