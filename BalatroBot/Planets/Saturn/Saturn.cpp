#include "Saturn.hpp"

Saturn::Saturn() : bot(), effect("Increases Straight hand value by +3 Mult and +30 Chips"), name("Saturn") {}

Saturn::Saturn(const Saturn& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Saturn::~Saturn() {}

void Saturn::printPlanet(){
	// Implementazione UI se necessaria
}

void Saturn::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Straight");
	highCardHand.setChips(highCardHand.getChips() + 30);
	highCardHand.setMult(highCardHand.getMult() + 3);
	highCardHand.setLevel(highCardHand.getLevel() + 1);
}

std::string Saturn::getEffect(){
	return effect;
}

std::string Saturn::getName(){
	return name;
}

