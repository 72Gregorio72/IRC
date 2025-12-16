#include "MysticSummit.hpp"

MysticSummit::MysticSummit() :bot(), cost(4), effect("+15 Mult when 0 discards remaining"), name("MysticSummit"), rarity("Common") {}

MysticSummit::~MysticSummit() {}

MysticSummit::MysticSummit(const MysticSummit &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void MysticSummit::printJoker(){
    // Implementazione UI se necessaria
}


void MysticSummit::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	int remainingDiscards = bot->getDiscards();
	if (remainingDiscards == 0) {
		mult += 15;
	}
}

int MysticSummit::getCost(){
	return cost;
}

std::string MysticSummit::getName() {
	return name;
}

std::string MysticSummit::getEffect() {
	return effect;
}

std::string MysticSummit::getRarity() {
	return rarity;
}