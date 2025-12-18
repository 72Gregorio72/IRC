#include "Jupiter.hpp"

Jupiter::Jupiter() : bot(), effect("Increases Flush hand value by +2 Mult and +15 Chips"), name("Jupiter") {}

Jupiter::Jupiter(const Jupiter& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Jupiter::~Jupiter() {}

void Jupiter::printPlanet(){
	// Implementazione UI se necessaria
}

void Jupiter::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Flush");
	highCardHand.setChips(highCardHand.getChips() + 15);
	highCardHand.setMult(highCardHand.getMult() + 2);
	highCardHand.setLevel(highCardHand.getLevel() + 1);
}

std::string Jupiter::getEffect(){
	return effect;
}

std::string Jupiter::getName(){
	return name;
}