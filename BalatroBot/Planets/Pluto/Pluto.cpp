#include "Pluto.hpp"

Pluto::Pluto() : bot(), effect("Increases High Card hand value by +1 Mult and +10 Chips"), name("Pluto") {}

Pluto::Pluto(const Pluto& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Pluto::~Pluto() {}

void Pluto::printPlanet(){
	// Implementazione UI se necessaria
}

void Pluto::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("High Card");
	highCardHand.setChips(highCardHand.getChips() + 10);
	highCardHand.setMult(highCardHand.getMult() + 1);
	highCardHand.setRank(highCardHand.getRank() + 1);
}

std::string Pluto::getEffect(){
	return effect;
}

std::string Pluto::getName(){
	return name;
}

