#include "BlackBoard.hpp"

BlackBoard::BlackBoard() :bot(), cost(6), effect("X3 Mult if all cards held in hand are Spade or Club"), name("Black Board"), rarity("Uncommon") {}

BlackBoard::~BlackBoard() {}

BlackBoard::BlackBoard(const BlackBoard &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void BlackBoard::printJoker(){
    // Implementazione UI se necessaria
}


void BlackBoard::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	bool allBlack = true;
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string suit = selectedCards[i].getSuit();
		if (suit != "Spade" && suit != "Club") {
			allBlack = false;
			break;
		}
	}
	if (allBlack) {
		mult *= 3;
	}
}

int BlackBoard::getCost(){
	return cost;
}

std::string BlackBoard::getName() {
	return name;
}

std::string BlackBoard::getEffect() {
	return effect;
}

std::string BlackBoard::getRarity() {
	return rarity;
}