#ifndef BALATRO_HPP
#define BALATRO_HPP

#include "../ft_irc.h"
#include "Card.hpp"

class Card;

class Balatro {
	public:
		Balatro();
		~Balatro();
		Balatro(const Balatro &other);
		Balatro &operator=(const Balatro &other);
		Balatro(int sd, User player);

		int getAnte() const;
		int getDiscards() const;
		std::vector<Card> getHand() const;
		std::vector<Card> getDeck() const;
		int getHands() const;
		int getCoins() const;
		int getCurrentBet() const;
		int getTotalBet() const;

		void setAnte(int newAnte);
		void setDiscards(int newDiscards);
		void setHand(const std::vector<Card> &newHand);
		void setDeck(const std::vector<Card> &newDeck);
		void setHands(int newHands);
		void setCoins(int newCoins);
		void setCurrentBet(int newCurrentBet);
		void setTotalBet(int newTotalBet);

		void startNewRound();
		void startNewGame();
		void shuffleDeck();
		void dealInitialHand();

		void printUI();
		std::vector<std::string> getCardRows(const Card& c);
		std::vector<std::string> printDeck();
		std::string centerText(std::string text, int width);
		void getLeftPanelContent(int row, std::string& raw, std::string& colored);
		std::string getRightPanelContent(int row, int handStart, int handH, int deckStart, int deckH, 
										const std::vector<std::vector<std::string> > & cardMatrix, 
										const std::vector<std::string>& deckVisual);
	private:
		int ante;
		int discards;
		std::vector<Card> hand;
		std::vector<Card> deck;
		int hands;
		int coins;
		int currentBet;
		int totalBet;
		int sd;
		User player;
};

#endif