#ifndef CARD_HPP
#define CARD_HPP

#include "ft_irc.h"

class Card {
	public:
		Card();
		~Card();
		Card(const Card &other);
		Card(const std::string &suit, const std::string &rank);
		Card &operator=(const Card &other);
		std::string getSuit() const;
		std::string getRank() const;

	private:
		std::string suit;
		std::string rank;
};

#endif