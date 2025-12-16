#include "Acrobat.hpp"

Acrobat::Acrobat() :bot(), cost(6), effect("X3 Mult on final hand of round"), name("Acrobat"), rarity("Uncommon") {}

Acrobat::~Acrobat() {}

Acrobat::Acrobat(const Acrobat &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Acrobat::printJoker(){
    // Implementazione UI se necessaria
}


void Acrobat::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	int roundNumber = bot->getHands();
	if (roundNumber == 1)
		mult *= 3;
}

int Acrobat::getCost(){
	return cost;
}

std::string Acrobat::getName() {
	return name;
}

std::string Acrobat::getEffect() {
	return effect;
}

std::string Acrobat::getRarity() {
	return rarity;
}