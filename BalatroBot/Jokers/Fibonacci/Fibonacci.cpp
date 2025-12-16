#include "Fibonacci.hpp"

Fibonacci::Fibonacci() :bot(), cost(8), effect("Each played Ace, 2, 3, 5, or 8 gives +8 Mult when scored"), name("Fibonacci"), rarity("Uncommon") {}

Fibonacci::~Fibonacci() {}

Fibonacci::Fibonacci(const Fibonacci &other) :bot(other.bot), cost(other.cost), effect(other.effect), name(other.name), rarity(other.rarity) {}

void Fibonacci::printJoker(){
    // Implementazione UI se necessaria
}


void Fibonacci::playJoker(int& chips, int& mult, Balatro *bot){
	(void)chips;
	std::vector<Card> selectedCards = bot->getSelectedCards();
	for (size_t i = 0; i < selectedCards.size(); ++i) {
		std::string rank = selectedCards[i].getRank();
		if (rank == "A" || rank == "2" || rank == "3" || rank == "5" || rank == "8") {
			mult += 8;
		}
	}
}

int Fibonacci::getCost(){
	return cost;
}

std::string Fibonacci::getName() {
	return name;
}

std::string Fibonacci::getEffect() {
	return effect;
}

std::string Fibonacci::getRarity() {
	return rarity;
}