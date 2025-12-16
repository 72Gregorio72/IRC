#include "HalfJoker.hpp"

HalfJoker::HalfJoker() :bot(), cost(4), effect("+20 Mult if played hand contains 3 or fewer cards"), name("Half Joker"), rarity("Common") {}

HalfJoker::~HalfJoker() {}

HalfJoker::HalfJoker(const HalfJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void HalfJoker::printJoker(){
    // Implementazione UI se necessaria
}


void HalfJoker::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	(void)bot;
	if (bot->getSelectedCards().size() <= 3) {
		mult += 20;
	}
}

int HalfJoker::getCost(){
	return cost;
}

std::string HalfJoker::getName() {
	return name;
}

std::string HalfJoker::getEffect() {
	return effect;
}

std::string HalfJoker::getRarity() {
	return rarity;
}