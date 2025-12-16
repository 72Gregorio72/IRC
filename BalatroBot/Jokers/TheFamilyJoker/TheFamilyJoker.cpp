#include "TheFamilyJoker.hpp"

TheFamilyJoker::TheFamilyJoker() :bot(), cost(8), effect("X3 Mult if played hand contains a Three of a Kind"), name("The Trio"), rarity("Rare") {}

TheFamilyJoker::~TheFamilyJoker() {}

TheFamilyJoker::TheFamilyJoker(const TheFamilyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void TheFamilyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void TheFamilyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Three of a Kind")
    	mult *= 3;
}

int TheFamilyJoker::getCost(){
	return cost;
}

std::string TheFamilyJoker::getName() {
	return name;
}

std::string TheFamilyJoker::getEffect() {
	return effect;
}

std::string TheFamilyJoker::getRarity() {
	return rarity;
}