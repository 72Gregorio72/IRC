#include "Earth.hpp"

Earth::Earth() : bot(), effect("Increases Full House hand value by +2 Mult and +25 Chips"), name("Earth") {}

Earth::Earth(const Earth& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Earth::~Earth() {}

void Earth::printPlanet(){
	// Implementazione UI se necessaria
}

void Earth::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Full House");
	highCardHand.setChips(highCardHand.getChips() + 25);
	highCardHand.setMult(highCardHand.getMult() + 2);
	highCardHand.setLevel(highCardHand.getLevel() + 1);
}

std::string Earth::getEffect(){
	return effect;
}

std::string Earth::getName(){
	return name;
}

