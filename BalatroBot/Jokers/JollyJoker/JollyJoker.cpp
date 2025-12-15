#include "JollyJoker.hpp"

JollyJoker::JollyJoker() :bot(), cost(3), effect("+8 mult if a Pair is present"), name("Jolly Joker"), rarity("Common") {}

JollyJoker::~JollyJoker() {}

JollyJoker::JollyJoker(const JollyJoker &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void JollyJoker::printJoker(){
    // Implementazione UI se necessaria
}


void JollyJoker::playJoker(int& chips, int& mult, Balatro *bot){
    (void)chips;
	(void)bot;
	if (bot->getBestHandName() == "Pair")
    	mult += 8;
}

int JollyJoker::getCost(){
	return cost;
}

std::string JollyJoker::getName() {
	return name;
}

std::string JollyJoker::getEffect() {
	return effect;
}

std::string JollyJoker::getRarity() {
	return rarity;
}