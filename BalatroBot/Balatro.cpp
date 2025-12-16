#include "includes/Balatro.hpp"
#include "Jokers/BaseJoker/BaseJoker.hpp"
#include "Jokers/GreedyJoker/GreedyJoker.hpp"
#include "Jokers/WrathfulJoker/WrathfulJoker.hpp"
#include "Jokers/LustyJoker/LustyJoker.hpp"
#include "Jokers/GluttonousJoker/GluttonousJoker.hpp"
#include "Jokers/JollyJoker/JollyJoker.hpp"
#include "Jokers/ZanyJoker/ZanyJoker.hpp"
#include "Jokers/MadJoker/MadJoker.hpp"
#include "Jokers/CrazyJoker/CrazyJoker.hpp"
#include "Jokers/DrollJoker/DrollJoker.hpp"
#include "Jokers/SlyJoker/SlyJoker.hpp"
#include "Jokers/WilyJoker/WilyJoker.hpp"
#include "Jokers/CleverJoker/CleverJoker.hpp"
#include "Jokers/DeviousJoker/DeviousJoker.hpp"
#include "Jokers/CraftyJoker/CraftyJoker.hpp"
#include "Jokers/TheDuoJoker/TheDuoJoker.hpp"
#include "Jokers/TheTrioJoker/TheTrioJoker.hpp"
#include "Jokers/TheFamilyJoker/TheFamilyJoker.hpp"
#include "Jokers/TheOrderJoker/TheOrderJoker.hpp"
#include "Jokers/TheTribeJoker/TheTribeJoker.hpp"
#include "Jokers/HalfJoker/HalfJoker.hpp"
#include "Jokers/Banner/Banner.hpp"
#include "Jokers/MysticSummit/MysticSummit.hpp"

Balatro::Balatro() : gameOver(false), gameWon(false), ante(1), anteScore(0), discards(4), hands(4), coins(0), currentBet(0), totalBet(0), sd(0), player(), pokerHands(), isSuitSorting(false), isCashingOut(false), isShopUI(false), jokers(), allJokers(), bestHandName(""), pendingShopIndex(-1), packJokers(), isInJokerPackUI(0) {
    std::srand(static_cast<unsigned int>(std::time(0)));
    this->rollPrice = 5;
}

Balatro::Balatro(int sd, User *player) : gameOver(false), gameWon(false), ante(1), anteScore(0), discards(4), hands(4), coins(0), currentBet(0), totalBet(0), sd(sd), player(player), pokerHands(), isRankSorting(false), isCashingOut(false), isShopUI(false), jokers(), allJokers(), bestHandName(""), pendingShopIndex(-1), packJokers(), isInJokerPackUI(0) {
    std::srand(static_cast<unsigned int>(std::time(0)));
    this->rollPrice = 5;
}

Balatro::~Balatro() {
    // 1. Svuota i vettori secondari SENZA fare delete (sono solo riferimenti)
    jokers.clear();
    shopJokers.clear();

    // 2. Cancella la memoria vera e propria dal "proprietario" (allJokers)
    for (size_t i = 0; i < allJokers.size(); ++i) {
        delete allJokers[i];
    }
    allJokers.clear();
}

Balatro::Balatro(const Balatro &other)
	: gameOver(other.gameOver),
      gameWon(other.gameWon),
	  ante(other.ante),
	  anteScore(other.anteScore),
	  discards(other.discards),
	  hand(other.hand),
	  deck(other.deck),
	  selectedCards(other.selectedCards),
	  hands(other.hands), 
	  coins(other.coins),
	  currentBet(other.currentBet),
	  totalBet(other.totalBet),
	  sd(other.sd),
	  player(other.player),
	  pokerHands(other.pokerHands),
	  isSuitSorting(other.isSuitSorting),
	  isRankSorting(other.isRankSorting), 
	  isCashingOut(other.isCashingOut),
	  isShopUI(other.isShopUI),
	  jokers(other.jokers),
	  bestHandName(other.bestHandName), 
	  pendingShopIndex(other.pendingShopIndex), 
	  rollPrice(other.rollPrice),
	  blind(other.blind),
	  packJokers(other.packJokers),
	  isInJokerPackUI(other.isInJokerPackUI) {}

Balatro &Balatro::operator=(const Balatro &other) {
	if (this != &other) {
		gameOver = other.gameOver;
        gameWon = other.gameWon;
		ante = other.ante;
		anteScore = other.anteScore;
		discards = other.discards;
		hand = other.hand;
		deck = other.deck;
		selectedCards = other.selectedCards;
		hands = other.hands;
		coins = other.coins;
		currentBet = other.currentBet;
		totalBet = other.totalBet;
		sd = other.sd;
		player = other.player;
		pokerHands = other.pokerHands;
		isSuitSorting = other.isSuitSorting;
		isRankSorting = other.isRankSorting;
		isCashingOut = other.isCashingOut;
		isShopUI = other.isShopUI;
		jokers = other.jokers;
		bestHandName = other.bestHandName;
		pendingShopIndex = other.pendingShopIndex;
        rollPrice = other.rollPrice;
		blind = other.blind;
		packJokers = other.packJokers;
		isInJokerPackUI = other.isInJokerPackUI;
	}
	return *this;
}

void Balatro::initPokerHands() {
	pokerHands.HighCard.setPokerHand("High Card", 1, 5, 1);
    pokerHands.OnePair.setPokerHand("Pair", 2, 10, 2);
    pokerHands.TwoPair.setPokerHand("Two Pair", 3, 20, 2);
    pokerHands.ThreeOfAKind.setPokerHand("Three of a Kind", 4, 30, 3);
    pokerHands.Straight.setPokerHand("Straight", 5, 30, 4);
    pokerHands.Flush.setPokerHand("Flush", 6, 35, 4);
    pokerHands.FullHouse.setPokerHand("Full House", 7, 40, 4);
    pokerHands.FourOfAKind.setPokerHand("Four of a Kind", 8, 60, 7);
    pokerHands.StraightFlush.setPokerHand("Straight Flush", 9, 100, 8);
    pokerHands.RoyalFlush.setPokerHand("Royal Flush", 10, 100, 8);
    pokerHands.FiveOfAKind.setPokerHand("Five of a Kind", 11, 120, 12);
    pokerHands.FlushHouse.setPokerHand("Flush House", 12, 140, 14);
    pokerHands.FlushFive.setPokerHand("Flush Five", 13, 160, 16);
}

const std::vector<Card>& Balatro::getSelectedCards() const {
    return selectedCards;
}

void Balatro::initAllJokers() {
	if (!allJokers.empty()) {
        return; 
    }

    allJokers.push_back(new BaseJoker());
    allJokers.push_back(new GreedyJoker());
    allJokers.push_back(new LustyJoker());
    allJokers.push_back(new WrathfulJoker());
    allJokers.push_back(new GluttonousJoker());
	allJokers.push_back(new BaseJoker());
	allJokers.push_back(new GreedyJoker());
	allJokers.push_back(new JollyJoker());
	allJokers.push_back(new ZanyJoker());
	allJokers.push_back(new MadJoker());
	allJokers.push_back(new CrazyJoker());
	allJokers.push_back(new DrollJoker());
	allJokers.push_back(new SlyJoker());
	allJokers.push_back(new WilyJoker());
	allJokers.push_back(new CleverJoker());
	allJokers.push_back(new DeviousJoker());
	allJokers.push_back(new CraftyJoker());
	allJokers.push_back(new TheDuoJoker());
	allJokers.push_back(new TheTrioJoker());
	allJokers.push_back(new TheFamilyJoker());
	allJokers.push_back(new TheOrderJoker());
	allJokers.push_back(new TheTribeJoker());
	allJokers.push_back(new HalfJoker());
	allJokers.push_back(new Banner());
	allJokers.push_back(new MysticSummit());
}

void Balatro::startNewRound() {
	blind++;
	if (blind > 2)
		blind = 0;
	anteScore = calculateAnteScore();
	hand.clear();
	deck.clear();
	selectedCards.clear();
	gameOver = false;
	currentBet = 0;
    rollPrice = 5;
	totalBet = 0;
	discards = 4;
	hands = 4;
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

int getRankValue(std::string rank) {
    if (rank == "J") return 11;
    if (rank == "Q") return 12;
    if (rank == "K") return 13;
    if (rank == "A") return 14;
    return std::atol(rank.c_str());
}

bool compareCardsByRank(const Card& a, const Card& b) {
    int valA = getRankValue(a.getRank());
    int valB = getRankValue(b.getRank());
    
    if (valA != valB) {
        return valA > valB;
    }
    return a.getSuit() < b.getSuit();
}

bool compareCardsBySuit(const Card& a, const Card& b) {
    if (a.getSuit() != b.getSuit()) {
        return a.getSuit() < b.getSuit();
    }
    return getRankValue(a.getRank()) > getRankValue(b.getRank());
}

int getRankChips(std::string rank) {
    if (rank == "J" || rank == "Q" || rank == "K") return 10;
    if (rank == "A") return 11;
    return std::atol(rank.c_str());
}

// ... (codice precedente invariato fino a calculateHand)
int Balatro::calculateHand() {
    if (selectedCards.empty()) {
        std::cout << "DEBUG: Nessuna carta selezionata." << std::endl;
        return 0;
    }

    std::cout << "\n--- DEBUG: INIZIO CALCOLO MANO ---" << std::endl;

    bool isFlush = true;
    std::string firstSuit = selectedCards[0].getSuit();
    int handChipsFromCards = 0;
    
    std::map<std::string, int> rankCounts;
    std::vector<int> values;

    std::cout << "DEBUG: Carte selezionate (" << selectedCards.size() << "):" << std::endl;

    for (size_t i = 0; i < selectedCards.size(); ++i) {
        Card currentCard = selectedCards[i];
        
        int rVal = getRankValue(currentCard.getRank());
        int rChips = getRankChips(currentCard.getRank());

        if (currentCard.getSuit() != firstSuit) {
            isFlush = false;
        }
        
        rankCounts[currentCard.getRank()]++;
        handChipsFromCards += rChips;
        values.push_back(rVal);
    }

    bool isStraight = false;
    if (selectedCards.size() == 5) {
        std::sort(values.begin(), values.end());
        bool sequential = true;
        for (size_t i = 0; i < values.size() - 1; ++i) {
            if (values[i] + 1 != values[i+1]) {
                sequential = false;
                break;
            }
        }
        if (!sequential && values[0]==2 && values[1]==3 && values[2]==4 && values[3]==5 && values[4]==14) {
            sequential = true;
        }
        isStraight = sequential;
    }

    bool hasFiveOfAKind = false;
    bool hasFourOfAKind = false;
    bool hasThreeOfAKind = false;
    bool hasPair = false;
    int pairCount = 0;

    for (std::map<std::string, int>::iterator it = rankCounts.begin(); it != rankCounts.end(); ++it) {
        int count = it->second;
        if (count == 5) hasFiveOfAKind = true;
        if (count == 4) hasFourOfAKind = true;
        if (count == 3) hasThreeOfAKind = true;
        if (count == 2) {
            hasPair = true;
            pairCount++;
        }
    }

    // --- IDENTIFICAZIONE MANO MIGLIORE ---
    PokerHand* bestHand = &pokerHands.HighCard; // Default

    if (hasFiveOfAKind && isFlush) {
        bestHand = &pokerHands.FlushFive;
		bestHandName = pokerHands.FlushFive.getName();
    } else if (isFlush && hasThreeOfAKind && hasPair) {
        bestHand = &pokerHands.FlushHouse;
		bestHandName = pokerHands.FlushHouse.getName();
    } else if (hasFiveOfAKind) {
        bestHand = &pokerHands.FiveOfAKind;
		bestHandName = pokerHands.FiveOfAKind.getName();
    } else if (isFlush && isStraight && values.back() == 14 && values[0] == 10) { 
        bestHand = &pokerHands.RoyalFlush;
		bestHandName = pokerHands.RoyalFlush.getName();
    } else if (isFlush && isStraight) {
        bestHand = &pokerHands.StraightFlush;
		bestHandName = pokerHands.StraightFlush.getName();
    } else if (hasFourOfAKind) {
        bestHand = &pokerHands.FourOfAKind;
		bestHandName = pokerHands.FourOfAKind.getName();
    } else if (hasThreeOfAKind && hasPair) {
        bestHand = &pokerHands.FullHouse;
		bestHandName = pokerHands.FullHouse.getName();
    } else if (isFlush && selectedCards.size() == 5) {
        bestHand = &pokerHands.Flush;
		bestHandName = pokerHands.Flush.getName();
    } else if (isStraight) {
        bestHand = &pokerHands.Straight;
		bestHandName = pokerHands.Straight.getName();
    } else if (hasThreeOfAKind) {
        bestHand = &pokerHands.ThreeOfAKind;
		bestHandName = pokerHands.ThreeOfAKind.getName();
    } else if (pairCount >= 2) {
        bestHand = &pokerHands.TwoPair;
		bestHandName = pokerHands.TwoPair.getName();
    } else if (hasPair) {
        bestHand = &pokerHands.OnePair;
		bestHandName = pokerHands.OnePair.getName();
    }
    int currentChips = bestHand->getChips() + handChipsFromCards;
    int currentMult = bestHand->getMult();

    std::cout << "DEBUG: Base Score -> Chips: " << currentChips << " Mult: " << currentMult << std::endl;

    for (size_t i = 0; i < jokers.size(); ++i) {
        if (jokers[i]) {
            jokers[i]->playJoker(currentChips, currentMult, this);
        }
    }

    std::cout << "DEBUG: Final Score -> Chips: " << currentChips << " Mult: " << currentMult << " = " << (currentChips * currentMult) << std::endl;

    return currentChips * currentMult;
}

void Balatro::pickJokerFromPack(int index) {
	if (index < 0 || index >= static_cast<int>(packJokers.size())) {
		std::cout << "DEBUG: Indice joker non valido." << std::endl;
		return;
	}
	IJoker* selectedJoker = packJokers[index];
	jokers.push_back(selectedJoker);
	packJokers.erase(packJokers.begin() + index);
	std::cout << "DEBUG: Joker " << selectedJoker->getName() << " aggiunto alla collezione." << std::endl;
}

void Balatro::getMessagePrompt(std::string msg) {

	if (gameOver) {
		std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Send /balatro to start a new round\r\n";
		send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
		return ;
	}
	std::cout << "BalatroBot message: " << msg << std::endl;
	if (msg.find("!") == 0) {
		msg.erase(0, 1);
		std::cout << "Received command: " << msg << std::endl;
		if (msg.find("select ") == 0) {
            if (isCashingOut || isShopUI) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :!select not possible\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
			msg.erase(0, 7);
			selectedCards.clear();
			std::vector<std::string> tokens;
			size_t pos = 0;
			while ((pos = msg.find(" ")) != std::string::npos) {
				tokens.push_back(msg.substr(0, pos));
				msg.erase(0, pos + 1);
			}
			tokens.push_back(msg);
			if (tokens.size() < 1) {
				std::cout << "No card index provided." << std::endl;
				return;
			}
			std::vector<int> cardIndices;
			for (size_t i = 0; i < tokens.size(); ++i) {
				try {
					int index = std::atol(tokens[i].c_str());
					index--;
					std::cout << "Parsed index: " << index << std::endl;
					if (index >= 8){
						std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Invalid card index: " + tokens[i] + "\r\n";
						send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
						return;
					}
					cardIndices.push_back(index);
				} catch (const std::invalid_argument&) {
					std::cout << "Invalid card index: " << tokens[i] << std::endl;
					return;
				}
			}
			if (cardIndices.size() > 5) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Too many cards selected\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return ;
			}
			for(size_t i = 0; i < cardIndices.size(); ++i) {
				std::cout << "Selected card: " << hand[cardIndices[i]].getRank() << " of " << hand[cardIndices[i]].getSuit() << std::endl;
			}
			for(size_t i = 0; i < cardIndices.size(); ++i) {
				selectedCards.push_back(hand[cardIndices[i]]);
			}
			printSelectedCardsUI();
		} else if (msg.find("discard") == 0) {
            if (isCashingOut || isShopUI) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :!discard not possible\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
			msg.erase(0, 7);
			if (selectedCards.empty()) {
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :No cards selected to discard.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			if (discards == 0){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You have finished your discards.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			for(size_t i = 0; i < selectedCards.size(); ++i) {
				for (size_t j = 0; j < hand.size(); ++j) {
					if (hand[j].getRank() == selectedCards[i].getRank() &&
						hand[j].getSuit() == selectedCards[i].getSuit()) {
						hand.erase(hand.begin() + j);
						break;
					}
				}
			}
			while (hand.size() < 8 && !deck.empty()) {
				hand.push_back(deck.back());
				deck.pop_back();
			}
			selectedCards.clear();
			discards--;
			if (isSuitSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsBySuit);
			if (isRankSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsByRank);
			printSelectedCardsUI();
		} else if (msg.find("play") == 0) {
            if (isCashingOut || isShopUI) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :!play not possible\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
			if (selectedCards.empty()) {
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :No cards selected to play.\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			msg.erase(0, 4);
			playHand();
			selectedCards.clear();
			if (isSuitSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsBySuit);
			if (isRankSorting)
            	std::sort(hand.begin(), hand.end(), compareCardsByRank);
		} else if (msg.find("sort suit") == 0) {
            if (isCashingOut || isShopUI) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :!sort not possible\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
            std::sort(hand.begin(), hand.end(), compareCardsBySuit);
            printSelectedCardsUI();
            isSuitSorting = true;
			isRankSorting = false;
        } else if (msg.find("sort rank") == 0) {
            if (isCashingOut || isShopUI) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :!sort not possible\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
            std::sort(hand.begin(), hand.end(), compareCardsByRank);
            printSelectedCardsUI();
			isSuitSorting = false;
			isRankSorting = true;
        } else if (msg.find("cash out") == 0) {
			if (!isCashingOut){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the cashing out screen\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			printShopUI();
			isCashingOut = false;
        } if (msg.find("reroll") == 0) {
            if (isShopUI){
                if (coins < rollPrice) {
                    std::string err = ":BalatroBot PRIVMSG " + player->getNickName() + " :Not enough coins to roll the shop\r\n";
                    send(sd, err.c_str(), err.length(), MSG_NOSIGNAL);
                    return;
                }
                coins -= rollPrice;
                rollPrice = rollPrice * 1.50f;
                generateShopJokers();
                printShopUI();
            }
        } else if (msg.find("next") == 0) {
			if (!isShopUI){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the shop screen\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			if (blind == 2)
				ante++;
			startNewRound();
			printSelectedCardsUI();
			isShopUI = false;
        } else if (msg.find("shopUI") == 0) {
            generateShopJokers();
			printShopUI();
        } else if (msg.find("shop") == 0) {
            if (!isShopUI) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the shop screen\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
            std::string params = "";
            if (msg.length() > 5) params = msg.substr(5);
            
            std::stringstream ss(params);
            int tempIndex;
            std::vector<int> indicesToBuy;

            while (ss >> tempIndex) {
                // Convertiamo da 1-based (utente) a 0-based (vector)
                indicesToBuy.push_back(tempIndex - 1);
            }

            if (indicesToBuy.empty()) {
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Usage: !shop <id> (e.g. !shop 1 3)\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }
            std::sort(indicesToBuy.begin(), indicesToBuy.end(), std::greater<int>());
            
            std::vector<int>::iterator it = std::unique(indicesToBuy.begin(), indicesToBuy.end());
            indicesToBuy.resize(std::distance(indicesToBuy.begin(), it));

            bool updateUI = false;

            for (size_t i = 0; i < indicesToBuy.size(); ++i) {
                int idx = indicesToBuy[i];

                if (idx < 0 || idx >= (int)shopJokers.size()) {
                    continue;
                }

                IJoker* targetJoker = shopJokers[idx];

                if (jokers.size() >= 5) {
                pendingShopIndex = idx;
                
                std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Max Jokers reached! Type !replace <1-5> to swap with " + targetJoker->getName() + " or ignore.\r\n";
                send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
                return;
            }

                if (coins < targetJoker->getCost()) {
                    std::string err = ":BalatroBot PRIVMSG " + player->getNickName() + " :Not enough coins for " + targetJoker->getName() + "\r\n";
                    send(sd, err.c_str(), err.length(), MSG_NOSIGNAL);
                    continue;
                }

                coins -= targetJoker->getCost();
                
                jokers.push_back(targetJoker);
                
                shopJokers.erase(shopJokers.begin() + idx);

                std::string success = ":BalatroBot PRIVMSG " + player->getNickName() + " :Bought " + targetJoker->getName() + "\r\n";
                send(sd, success.c_str(), success.length(), MSG_NOSIGNAL);
                
                updateUI = true;
            }
            if (updateUI) {
                printShopUI();
            }
        } else if (msg.find("replace") == 0) {
            // 1. Controllo se c'è un acquisto in sospeso
            if (pendingShopIndex == -1) {
                std::string err = ":BalatroBot PRIVMSG " + player->getNickName() + " :No purchase pending. Use !shop first.\r\n";
                send(sd, err.c_str(), err.length(), MSG_NOSIGNAL);
                return;
            }

            // 2. Controllo se il joker nello shop esiste ancora (safety check)
            if (pendingShopIndex >= (int)shopJokers.size()) {
                pendingShopIndex = -1; // Reset
                printShopUI(); 
                return;
            }

            // 3. Parsing dell'indice da sostituire (1-5)
            msg.erase(0, 8); // rimuove "!replace"
            int replaceIdx = std::atoi(msg.c_str()); 
            replaceIdx--; // 0-based

            if (replaceIdx < 0 || replaceIdx >= (int)jokers.size()) {
                std::string err = ":BalatroBot PRIVMSG " + player->getNickName() + " :Invalid Joker index to replace.\r\n";
                send(sd, err.c_str(), err.length(), MSG_NOSIGNAL);
                return;
            }

            IJoker* newJoker = shopJokers[pendingShopIndex];
            IJoker* oldJoker = jokers[replaceIdx];

            // 4. Esecuzione Transazione
            coins -= newJoker->getCost();
            
            // Rimuovo il vecchio joker dalla mano
            jokers.erase(jokers.begin() + replaceIdx);
            
            // Aggiungo il nuovo joker
            jokers.push_back(newJoker);

            // Rimuovo il nuovo joker dallo shop
            shopJokers.erase(shopJokers.begin() + pendingShopIndex);

            // 5. Reset e Feedback
            pendingShopIndex = -1; // Transazione completata
            
            std::string success = ":BalatroBot PRIVMSG " + player->getNickName() + " :Replaced " + oldJoker->getName() + " with " + newJoker->getName() + "\r\n";
            send(sd, success.c_str(), success.length(), MSG_NOSIGNAL);
            
            printShopUI();
        } else if (msg.find("pack") == 0) {
			if (!isShopUI){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the shop screen\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			int pack = std::atoi(msg.substr(5).c_str());
			if (pack == 1)
				jokerPackUI();
			else if (pack == 2){
				//planetPackUI();
			}
			else {
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Invalid pack number\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
		} else if (msg.find("pick") == 0) {
			if (!isInJokerPackUI){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the joker pack screen\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			int pick = std::atoi(msg.substr(5).c_str());
			if (pick < 1 || pick > (int)packJokers.size()){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Invalid pick number\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			if (coins < 6){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Not enough coins to pick a joker\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			coins -= 6;
			pickJokerFromPack(pick - 1);
			isInJokerPackUI = 0;
			printShopUI();
		} else if (msg.find("skip") == 0) {
			if (!isInJokerPackUI){
				std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :You are not in the joker pack screen\r\n";
				send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
				return;
			}
			packJokers.clear();
			isInJokerPackUI = 0;
			printShopUI();
		}
	}
}

void Balatro::playHand() {
	totalBet += calculateHand();
	for(size_t i = 0; i < selectedCards.size(); ++i) {
		for (size_t j = 0; j < hand.size(); ++j) {
			if (hand[j].getRank() == selectedCards[i].getRank() &&
				hand[j].getSuit() == selectedCards[i].getSuit()) {
				hand.erase(hand.begin() + j);
				break;
			}
		}
	}
	while (hand.size() < 8 && !deck.empty()) {
		hand.push_back(deck.back());
		deck.pop_back();
	}
	selectedCards.clear();
	if (totalBet >= anteScore){
		printEndRoundUI();
		return ;
	}
	hands--;
	if (isSuitSorting)
		std::sort(hand.begin(), hand.end(), compareCardsBySuit);
	if (isRankSorting)
		std::sort(hand.begin(), hand.end(), compareCardsByRank);
	printSelectedCardsUI();
	if (hands == 0)
	{
		std::string msg = ":BalatroBot PRIVMSG " + player->getNickName() + " :Game Over\r\n";
		send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
		setGameOver(true);
		printUI();
		freeJokers();
		return ;
	}
	std::cout << "current score: " << totalBet << std::endl;
}
