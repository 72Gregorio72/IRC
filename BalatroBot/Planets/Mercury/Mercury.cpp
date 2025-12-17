#include "Mercury.hpp"

Mercury::Mercury() : bot(), effect("Increases Pair hand value by +1 Mult and +15 Chips"), name("Mercury") {}

Mercury::Mercury(const Mercury& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Mercury::~Mercury() {}

void Mercury::printPlanet(){
	// Implementazione UI se necessaria
}

void Mercury::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Pair");
	highCardHand.setChips(highCardHand.getChips() + 15);
	highCardHand.setMult(highCardHand.getMult() + 1);
	highCardHand.setRank(highCardHand.getRank() + 1);
}

std::string Mercury::getEffect(){
	return effect;
}

std::string Mercury::getName(){
	return name;
}

