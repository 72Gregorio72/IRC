#include "Neptune.hpp"

Neptune::Neptune() : bot(), effect("Increases Straight Flush hand value by +4 Mult and +40 Chips"), name("Neptune") {}

Neptune::Neptune(const Neptune& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Neptune::~Neptune() {}

void Neptune::printPlanet(){
	// Implementazione UI se necessaria
}

void Neptune::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Straight Flush");
	highCardHand.setChips(highCardHand.getChips() + 40);
	highCardHand.setMult(highCardHand.getMult() + 4);
	highCardHand.setRank(highCardHand.getRank() + 1);
}

std::string Neptune::getEffect(){
	return effect;
}

std::string Neptune::getName(){
	return name;
}

