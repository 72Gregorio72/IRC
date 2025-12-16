#include "Misprint.hpp"

Misprint::Misprint() :bot(), cost(4), effect("+0-23 Mult"), name("Misprint"), rarity("Common") {}

Misprint::~Misprint() {}

Misprint::Misprint(const Misprint &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Misprint::printJoker(){
    // Implementazione UI se necessaria
}


void Misprint::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	(void)bot;
	int randomMult = std::rand() % 24; // Genera un numero casuale tra 0 e 23
	mult += randomMult;
}

int Misprint::getCost(){
	return cost;
}

std::string Misprint::getName() {
	return name;
}

std::string Misprint::getEffect() {
	return effect;
}

std::string Misprint::getRarity() {
	return rarity;
}