#include "Uranus.hpp"

Uranus::Uranus() : bot(), effect("Increases Two Pair hand value by +1 Mult and +20 Chips"), name("Uranus") {}

Uranus::Uranus(const Uranus& other) : bot(other.bot), effect(other.effect), name(other.name) {}

Uranus::~Uranus() {}

void Uranus::printPlanet(){
	// Implementazione UI se necessaria
}

void Uranus::playPlanet(Balatro *bot){
	PokerHand& highCardHand = bot->getPokerHands("Two Pair");
	highCardHand.setChips(highCardHand.getChips() + 20);
	highCardHand.setMult(highCardHand.getMult() + 1);
	highCardHand.setLevel(highCardHand.getLevel() + 1);
}

std::string Uranus::getEffect(){
	return effect;
}

std::string Uranus::getName(){
	return name;
}

