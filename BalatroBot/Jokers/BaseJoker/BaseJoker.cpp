#include "BaseJoker.hpp"

BaseJoker::BaseJoker() : cost(0) {}

BaseJoker::~BaseJoker() {}

BaseJoker::BaseJoker(const BaseJoker &other) : cost(other.cost) {}

void BaseJoker::printJoker(){
    // Implementazione UI se necessaria
}

// Logica specifica: Aggiunge +4 al Mult
void BaseJoker::playJoker(int& chips, int& mult){
    (void)chips; // Evita warning unused parameter
    mult += 4;
    std::cout << "DEBUG: BaseJoker attivato! Mult +4. Nuovo Mult: " << mult << std::endl;
}

int BaseJoker::getCost(){
	return cost;
}