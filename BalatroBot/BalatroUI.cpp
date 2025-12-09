#include "Balatro.hpp"

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string getSpaces(int count) {
    if (count < 0) return "";
    return std::string(count, ' ');
}

std::vector<std::string> Balatro::getCardRows(const Card& c) {
    std::vector<std::string> rows;
    std::string rank = c.getRank();
    std::string suit = c.getSuit();
    
    std::string RED = "\x03" "04";
    std::string ORANGE = "\x03" "07";
    std::string BLACK = "\x03" "01";
    std::string BLUE = "\x03" "12";
    std::string RESET = "\x0f";

    std::string color = BLACK; 
    std::string symbol = "";
    
    if (suit == "Hearts")        { symbol = "♥"; color = RED; }
    else if (suit == "Diamonds") { symbol = "♦"; color = ORANGE; }
    else if (suit == "Spades")   { symbol = "♠"; color = BLACK; }
    else if (suit == "Clubs")    { symbol = "♣"; color = BLUE; }
    
    std::string sym = color + symbol; 

    std::string rL = rank + (rank == "10" ? "" : " ");
    std::string rR = (rank == "10" ? "" : " ") + rank;

    // Pattern base
    std::string mid = "     " + sym + "     ";
    std::string col = "  " + sym + "     " + sym + "  ";
	std::string midCol = "  " + sym + "  " + sym + "  " + sym + "  ";

    std::string line[5]; 
    for(int i=0; i<5; i++) line[i] = "           "; 

    if (rank == "J" || rank == "Q" || rank == "K") {
        line[1] = "     " + color + rank + "     "; 
        line[2] = "     " + sym + "     ";
        line[3] = "     " + color + rank + "     ";
    }
    else if (rank == "A") {
        line[2] = mid;
    }
    else {
        int r = 0;
        if (rank == "2") r=2; else if (rank == "3") r=3; else if (rank == "4") r=4;
        else if (rank == "5") r=5; else if (rank == "6") r=6; else if (rank == "7") r=7;
        else if (rank == "8") r=8; else if (rank == "9") r=9; else if (rank == "10") r=10;

        if (r == 9) {
            line[0] = col;
            line[1] = col;
            line[2] = mid;
            line[3] = col;
            line[4] = col;
        }
        else if (r == 10) {
            line[0] = col;
            line[1] = midCol;
            line[2] = mid;
            line[3] = col;
            line[4] = col;
        }
        else {
            if (r >= 4) { line[0] = col; line[4] = col; }
            
            if (r == 6 || r == 7 || r == 8) { line[2] = col; }
            
            if (r == 2 || r == 3) { line[0] = mid; line[4] = mid; }
            
            if (r == 3 || r == 5) { line[2] = mid; }
            
            if (r == 7) { line[1] = mid; }
            
            if (r == 8) { line[1] = mid; line[3] = mid; }
        }
    }

    rows.push_back(color + "┌───────────┐" + RESET);      
    rows.push_back(color + "│ " + rL + "        │" + RESET); 
    rows.push_back(color + "│" + line[0] + "│" + RESET);    
    rows.push_back(color + "│" + line[1] + "│" + RESET);    
    rows.push_back(color + "│" + line[2] + "│" + RESET);    
    rows.push_back(color + "│" + line[3] + "│" + RESET);    
    rows.push_back(color + "│" + line[4] + "│" + RESET);    
    rows.push_back(color + "│        " + rR + " │" + RESET); 
    rows.push_back(color + "└───────────┘" + RESET);

    return rows;
}

std::vector<std::string> Balatro::printDeck() {
    std::vector<std::string> rows;
    
    std::string DARK_BLUE = "\x03" "02";
    std::string LIGHT_BLUE = "\x03" "12";
    std::string BLACK = "\x03" "01";
    std::string LINE_BG = "\x03" "01,02"; 
    std::string RESET = "\x0f";
    std::string BLOCK = "█";

    int width = 18; 
    int height = 12;

    double cx = (width - 1) / 2.0;
    double cy = (height - 1) / 2.0;
    
    double rx = 5.0; 
    double ry = 3.5; 

    rows.push_back(BLACK + "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" + RESET);

    for (int y = 0; y < height; ++y) {
        std::string line = "";
        for (int x = 0; x < width; ++x) {
            double dx = x - cx;
            double dy = y - cy;
            double dist = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);

            if (x == 2 && y == 1) {
                line += LINE_BG + "╠";
            }
            else if (x == width - 3 && y == 1) {
                line += LINE_BG + "╦";
            }
            else if (x == 2 && y == height - 2) {
                line += LINE_BG + "╩";
            }
            else if (x == width - 3 && y == height - 2) {
                line += LINE_BG + "╣";
            }
            else if (x == 2 && y <= height - 2) {
                line += LINE_BG + "║";
            }
            else if (x == width - 3 && y >= 1) {
                line += LINE_BG + "║";
            }
            else if (y == 1 && x >= 2) {
                line += LINE_BG + "═";
            }
            else if (y == height - 2 && x <= width - 3) {
                line += LINE_BG + "═";
            }
            else {
                if (dist <= 1.0) {
                    line += RESET + BLACK + BLOCK;
                } else {
                    line += RESET + DARK_BLUE + BLOCK;
                }
            }
        }
        rows.push_back(BLACK + "█" + line + BLACK + "█" + RESET);
    }

    rows.push_back(BLACK + "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" + RESET);

    return rows;
}

std::string Balatro::centerText(std::string text, int width) {
    int len = text.length();
    if (len >= width) return text;
    int left = (width - len) / 2;
    int right = width - len - left;
    return std::string(left, ' ') + text + std::string(right, ' ');
}

void Balatro::getLeftPanelContent(int row, std::string& rawOut, std::string& colOut) {
    // Definizioni colori
    std::string C_RED     = "\x03" "04";
    std::string C_BLUE    = "\x03" "12";
    std::string C_ORANGE  = "\x03" "07";
    std::string C_YELLOW  = "\x03" "08";
    std::string C_WHITE   = "\x03" "00";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // Valori di default (vuoti)
    rawOut = ""; 
    colOut = "";

    // --- LOGICA PANNELLO SINISTRO ---
    
    // GOAL CHIPS (In alto)
    if (row == 2) {
        rawOut = "         GOAL CHIPS          ";
        colOut = "         " + C_RED + BOLD + "GOAL CHIPS" + RESET + "          ";
    }
    else if (row == 3) {
        std::string tVal = "300"; // Qui potresti mettere una variabile targetScore
        std::string cVal = centerText(tVal, 29);
        rawOut = cVal;
        colOut = C_RED + BOLD + cVal + RESET;
    }
    
    // ROUND SCORE
    else if (row == 5) {
        rawOut = "        ROUND SCORE          ";
        colOut = "        " + C_WHITE + "ROUND SCORE" + RESET + "          ";
    }
    else if (row == 6) {
        std::string sVal = toString(currentBet);
        std::string cVal = centerText(sVal, 29);
        rawOut = cVal; 
        colOut = C_WHITE + BOLD + cVal + RESET;
    }

    // MONEY / COINS ($)
    else if (row == 9) {
        rawOut = "              $              ";
        colOut = "              " + C_YELLOW + BOLD + "$" + RESET + "              ";
    }
    else if (row == 10) {
        rawOut = "          +------+           ";
        colOut = "          " + C_YELLOW + "┌──────┐" + RESET + "           ";
    }
    else if (row == 11) {
        std::string cVal = centerText("$" + toString(coins), 6);
        rawOut = "          |      |           ";
        colOut = "          " + C_YELLOW + "│" + RESET + BOLD + cVal + RESET + C_YELLOW + "│" + RESET + "           ";
    }
    else if (row == 12) {
        rawOut = "          +------+           ";
        colOut = "          " + C_YELLOW + "└──────┘" + RESET + "           ";
    }

    // HANDS & DISCARDS (In basso)
    else if (row == 16) {
        rawOut = "    HANDS       DISC    ";
        colOut = "    " + C_BLUE + BOLD + "HANDS" + RESET + "       " + C_RED + BOLD + "DISC" + RESET + "    ";
    }
    else if (row == 17) {
        rawOut = "   +-----+     +-----+  ";
        colOut = "   " + C_BLUE + "┌─────┐" + RESET + "     " + C_RED + "┌─────┐" + RESET + "  ";
    }
    else if (row == 18) {
        std::string hVal = centerText(toString(hands), 5);
        std::string dVal = centerText(toString(discards), 5);
        rawOut = "   |     |     |     |  ";
        colOut = "   " + C_BLUE + "│" + RESET + BOLD + hVal + RESET + C_BLUE + "│" + RESET + "     " + C_RED + "│" + RESET + BOLD + dVal + RESET + C_RED + "│" + RESET + "  ";
    }
    else if (row == 19) {
        rawOut = "   +-----+     +-----+  ";
        colOut = "   " + C_BLUE + "└─────┘" + RESET + "     " + C_RED + "└─────┘" + RESET + "  ";
    }

    // ANTE (Ultimo in basso)
    else if (row == 22) {
        rawOut = "            ANTE             ";
        colOut = "            " + C_ORANGE + BOLD + "ANTE" + RESET + "             ";
    }
    else if (row == 23) {
        rawOut = "          +------+           ";
        colOut = "          " + C_ORANGE + "┌──────┐" + RESET + "           ";
    }
    else if (row == 24) {
        std::string aVal = centerText(toString(ante) + "/8", 6);
        rawOut = "          |      |           ";
        colOut = "          " + C_ORANGE + "│" + RESET + BOLD + aVal + RESET + C_ORANGE + "│" + RESET + "           ";
    }
    else if (row == 25) {
        rawOut = "          +------+           ";
        colOut = "          " + C_ORANGE + "└──────┘" + RESET + "           ";
    }
}

std::string Balatro::getRightPanelContent(int row, int handStart, int handH, int deckStart, int deckH, 
                                          const std::vector<std::vector<std::string> >& cardMatrix, 
                                          const std::vector<std::string>& deckVisual) {
    std::string right = "";

    // Render della Mano
    if (row >= handStart && row < handStart + handH) {
        int slice = row - handStart;
        for(size_t c = 0; c < cardMatrix.size(); c++) {
            right += cardMatrix[c][slice] + " ";
        }
    }
    // Numeri sotto le carte
    else if (row == handStart + handH) {
        for(size_t c = 0; c < hand.size(); c++) {
            std::string cardNum = "(" + toString(c + 1) + ")";
            int paddingLen = (13 - cardNum.length()) / 2;
            right += getSpaces(paddingLen) + cardNum + getSpaces(13 - paddingLen - cardNum.length()) + " ";
        }
    }
    // Spazio vuoto se non ci sono carte in quel punto
    else {
        right += getSpaces(hand.size() * 14);
    }

    // Render del Deck
    if (row >= deckStart && row < deckStart + deckH) {
        int dSlice = row - deckStart;
        right += "                                " + deckVisual[dSlice];
    }
    else if (row == deckStart + deckH) {
        std::string deckLabel = "(" + toString(deck.size()) + ")";
        int dPad = (26 - deckLabel.length()) / 2; 
        right += "                              " + getSpaces(dPad) + deckLabel + getSpaces(26 - dPad - deckLabel.length());
    }

    return right;
}

void Balatro::printUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player.getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29; 

    // 1. Preparazione grafica carte e mazzo
    std::vector< std::vector<std::string> > cardMatrix;
    for(size_t i = 0; i < hand.size(); i++) {
        cardMatrix.push_back(getCardRows(hand[i]));
    }
    std::vector<std::string> deckVisual = printDeck();
    
    // 2. Calcolo altezze e offset
    int handHeight = (!cardMatrix.empty() && !cardMatrix[0].empty()) ? cardMatrix[0].size() : 0;
    int deckHeight = deckVisual.size();

    int bottomBase = totalRows - 3; 
    int handStartRow = bottomBase - handHeight;
    int deckStartRow = bottomBase - deckHeight;

    std::string msg = "";

    // 3. Header pulizia e bordi
    for(int i=0; i<10; i++) msg += prefix + " \r\n";
	msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
	msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    // 4. Ciclo principale di rendering righe
    for (int row = 0; row < totalRows; ++row) {
        
        // A. Ottieni pannello sinistro
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);

        // B. Calcola padding per allineamento verticale
        std::string padding = getSpaces(leftColWidth - leftRaw.length());
        std::string leftPanel = leftColor + padding;

        // C. Ottieni pannello destro
        std::string rightPanel = getRightPanelContent(row, handStartRow, handHeight, deckStartRow, deckHeight, cardMatrix, deckVisual);
        
        msg += prefix + " " + leftPanel + " ║ " + "                             " + rightPanel + "\r\n";
    }

    // 5. Chiusura bordi
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";

    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}