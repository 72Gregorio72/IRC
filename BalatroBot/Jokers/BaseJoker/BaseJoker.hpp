#ifndef BASEJOKER_HPP
#define BASEJOKER_HPP

#include "../IJoker.hpp"

class BaseJoker : public IJoker {
	public:
		BaseJoker();
		BaseJoker(const BaseJoker& other);
		~BaseJoker();

        void printJoker();
        void playJoker(int& chips, int& mult);

		int getCost();
	private:
		int cost;
};

#endif