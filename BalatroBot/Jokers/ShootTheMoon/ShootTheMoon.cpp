#include "ShootTheMoon.hpp"

ShootTheMoon::ShootTheMoon() :bot(), cost(5), effect("Each Queen held in hand gives +13 Mult"), name("Shoot The Moon"), rarity("Common") {}

ShootTheMoon::~ShootTheMoon() {}

ShootTheMoon::ShootTheMoon(const ShootTheMoon &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void ShootTheMoon::printJoker(){
    // Implementazione UI se necessaria
}


void ShootTheMoon::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	int queenCount = 0;
	std::vector<Card> handCards = bot->getHandCards();
	for (size_t i = 0; i < handCards.size(); ++i) {
		std::string rank = handCards[i].getRank();
		if (rank == "Q") {
			queenCount++;
		}
	}
	mult += queenCount * 13;
}

int ShootTheMoon::getCost(){
	return cost;
}

std::string ShootTheMoon::getName() {
	return name;
}

std::string ShootTheMoon::getEffect() {
	return effect;
}

std::string ShootTheMoon::getRarity() {
	return rarity;
}