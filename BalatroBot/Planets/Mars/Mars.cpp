#include "Mars.hpp"

Mars::Mars() : bot(), effect("Increases Four of a Kind hand value by +3 Mult and +30 Chips"), name("Mars") {}

Mars::Mars(const Mars& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Mars::~Mars() {}

void Mars::printPlanet(){
	// Implementazione UI se necessaria
}

void Mars::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Four of a Kind");
	highCardHand.setChips(highCardHand.getChips() + 30);
	highCardHand.setMult(highCardHand.getMult() + 3);
}

std::string Mars::getEffect(){
	return effect;
}

std::string Mars::getName(){
	return name;
}

