#include "BlueJoker.hpp"

BlueJoker::BlueJoker() :bot(), cost(5), effect("+2 Chips for each remaining card in deck"), name("Blue Joker"), rarity("Common") {}

BlueJoker::~BlueJoker() {}

BlueJoker::BlueJoker(const BlueJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void BlueJoker::printJoker(){
    // Implementazione UI se necessaria
}


void BlueJoker::playJoker(int& chips, int& mult, Balatro *bot){
	(void)mult;
	int remainingCards = bot->getDeck().size();
	chips += remainingCards * 2;
}

int BlueJoker::getCost(){
	return cost;
}

std::string BlueJoker::getName() {
	return name;
}

std::string BlueJoker::getEffect() {
	return effect;
}

std::string BlueJoker::getRarity() {
	return rarity;
}