#include "Venus.hpp"

Venus::Venus() : bot(), effect("Increases Three of a Kind hand value by +2 Mult and +20 Chips"), name("Venus") {}

Venus::Venus(const Venus& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Venus::~Venus() {}

void Venus::printPlanet(){
	// Implementazione UI se necessaria
}

void Venus::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Three of a Kind");
	highCardHand.setChips(highCardHand.getChips() + 20);
	highCardHand.setMult(highCardHand.getMult() + 2);
	highCardHand.setLevel(highCardHand.getLevel() + 1);
}

std::string Venus::getEffect(){
	return effect;
}

std::string Venus::getName(){
	return name;
}

