#include "Balatro.hpp"

Balatro::Balatro() : ante(0), discards(0), hands(0), coins(0), currentBet(0), totalBet(0), sd(0), player() {}

Balatro::Balatro(int sd, User player) : ante(0), discards(0), hands(0), coins(0), currentBet(0), totalBet(0), sd(sd), player(player) {}

Balatro::~Balatro() {}

Balatro::Balatro(const Balatro &other)
	: ante(other.ante),
	  discards(other.discards),
	  hand(other.hand),
	  deck(other.deck),
	  hands(other.hands), 
	  coins(other.coins),
	  currentBet(other.currentBet),
	  totalBet(other.totalBet) {}

Balatro &Balatro::operator=(const Balatro &other) {
	if (this != &other) {
		ante = other.ante;
		discards = other.discards;
		hand = other.hand;
		deck = other.deck;
		hands = other.hands;
		coins = other.coins;
		currentBet = other.currentBet;
		totalBet = other.totalBet;
	}
	return *this;
}

void Balatro::startNewRound() {
	hand.clear();
	deck.clear();
	currentBet = 0;
	totalBet = 0;
	discards = 0;
	hands = 0;
	std::string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
	std::string ranks[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
	for (size_t i = 0; i < 4; ++i) {
		for (size_t j = 0; j < 13; ++j) {
			deck.push_back(Card(suits[i], ranks[j]));
		}
	}
	shuffleDeck();
	dealInitialHand();
}

void Balatro::dealInitialHand() {
    hand.clear();
    for (int i = 0; i < 8 && !deck.empty(); ++i) {
        hand.push_back(deck.back());
        deck.pop_back();
    }
    printUI();
}

void Balatro::shuffleDeck(){
	std::random_shuffle(deck.begin(), deck.end());
}

void Balatro::startNewGame() {
	startNewRound();
	coins = 0;
	ante = 0;
	discards = 0;
	hands = 0;
}