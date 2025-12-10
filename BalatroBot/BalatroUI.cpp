#include "Balatro.hpp"

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
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
		std::cout << "ante score: " << anteScore << std::endl;
		std::stringstream ss;
		ss << anteScore;
        std::string tVal = ss.str();
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
        std::string sVal = toString(totalBet);
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
    
    // --- CONFIGURAZIONE DIMENSIONI ---
    int boxWidth = 44;          // Larghezza del box comandi
    int targetRightCol = 85;    // Colonna base preferita per l'inizio del box
    int cardWidth = 14;         // Larghezza visiva di una carta (13 char + 1 spazio)

    // --- CALCOLO ALLINEAMENTO VERTICALE (CRUCIALE) ---
    // Calcoliamo la larghezza VISIVA che la mano occupa (o occuperebbe).
    // Questo serve per mantenere il box allineato verticalmente anche nelle righe vuote.
    int currentHandWidth = hand.size() * cardWidth;
    
    // La colonna dove inizia il box è il massimo tra il target (85) e (larghezza mano + padding).
    // Se la mano è molto larga, sposta il box a destra per tutte le righe.
    int boxStartCol = targetRightCol;
    if (currentHandWidth + 4 > targetRightCol) {
        boxStartCol = currentHandWidth + 4;
    }

    // --- COLORI ---
    std::string C_ORANGE  = "\x03" "07";
    std::string C_RED     = "\x03" "04";
    std::string C_BLUE    = "\x03" "12";
    std::string C_GREEN   = "\x03" "03";
    std::string C_YELLOW  = "\x03" "08";
    std::string C_GREY    = "\x03" "14";
    std::string C_WHITE   = "\x03" "00";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // --- 1. COSTRUZIONE CONTENUTO SINISTRO (MANO) ---
    std::string leftPart = "";
    int visibleLeftLen = 0; 

    // A. Righe delle carte
    if (row >= handStart && row < handStart + handH) {
        int slice = row - handStart;
        for(size_t c = 0; c < cardMatrix.size(); c++) {
            leftPart += cardMatrix[c][slice] + " ";
        }
        visibleLeftLen = currentHandWidth; // Qui la larghezza è quella delle carte
    }
    // B. Riga dei numeri sotto le carte
    else if (row == handStart + handH) {
        for(size_t c = 0; c < hand.size(); c++) {
            std::string cardNum = "(" + toString(c + 1) + ")";
            int paddingLen = (13 - cardNum.length()) / 2;
            std::string item = std::string(paddingLen, ' ') + cardNum + std::string(13 - paddingLen - cardNum.length(), ' ') + " ";
            leftPart += item;
        }
        visibleLeftLen = currentHandWidth;
    }
    // C. Righe vuote (sopra o sotto la mano) -> visibleLeftLen resta 0

    // --- 2. CALCOLO SPAZIATURA (FIX) ---
    // Calcoliamo gli spazi necessari per arrivare ESATTAMENTE a boxStartCol.
    // Se visibleLeftLen è 0 (righe vuote), stamperà tutti gli spazi necessari (boxStartCol).
    // Se visibleLeftLen è piena (righe carte), stamperà solo la differenza.
    int paddingNeeded = boxStartCol - visibleLeftLen;
    if (paddingNeeded < 0) paddingNeeded = 0;

    std::string spacer = std::string(paddingNeeded, ' ');

    // --- 3. COSTRUZIONE CONTENUTO DESTRO (BOX o DECK) ---
    std::string rightPart = "";
    std::string vBorder = C_ORANGE + "│" + RESET; 

    // A. ZONA BOX COMANDI (Dalla cima fino a toccare il mazzo)
    if (row < deckStart) {
        // Bordo Superiore
        if (row == 0) {
            rightPart = C_ORANGE + "┌──────────────────────────────────────────┐" + RESET;
        }
        // Bordo Inferiore (Solo se siamo esattamente sopra il mazzo)
        else if (row == deckStart - 1) {
             rightPart = C_ORANGE + "└──────────────────────────────────────────┘" + RESET;
        }
        // Contenuto Interno
        else {
            std::string text = "";
            int r = row; 

            // Definisci il contenuto delle righe
            if (r == 1)      text = BOLD + centerText("COMMANDS", boxWidth - 2) + RESET;
            else if (r == 2) text = C_ORANGE + "├──────────────────────────────────────────┤" + RESET;
            else if (r == 3) text = " " + C_GREEN + "!select" + RESET + " <id1> <id2>" + C_GREY + "  (es: !select 1 3 4) " + RESET;
            else if (r == 5) text = " " + C_RED + "!discard" + C_GREY + "  (Discard selection)            " + RESET;
            else if (r == 7) text = " " + C_BLUE + "!play" + C_GREY + "  (Play selection)                  " + RESET;
            else if (r == 9) text = " " + C_YELLOW + "!sort suit/rank" + C_GREY + "  (Order your hand)       " + RESET;
            else {
                // Righe vuote riempitive fino al deck
                text = std::string(boxWidth - 2, ' ');
            }

            // Gestione bordi laterali
            if (r == 2) {
                rightPart = text;
            } else {
                if (r > 10) text = std::string(boxWidth - 2, ' ');
                rightPart = vBorder + text + vBorder;
            }
        }
    }
    // B. ZONA DECK (Sotto il box)
    else if (row >= deckStart && row < deckStart + deckH) {
        int dSlice = row - deckStart;
        std::string deckRow = deckVisual[dSlice];
        
        // Centriamo il mazzo rispetto al box
        int deckPadLen = (boxWidth - 20) / 2; // 20 è circa la larghezza del mazzo
        if (deckPadLen < 0) deckPadLen = 0;
        std::string deckPad = std::string(deckPadLen, ' ');
        rightPart = deckPad + deckRow;
    }
    else if (row == deckStart + deckH) {
        std::string deckLabel = "(" + toString(deck.size()) + ")";
        int labelPadLen = (boxWidth - deckLabel.length()) / 2;
        rightPart = std::string(labelPadLen, ' ') + deckLabel; 
    }

    // Ritorna la riga completa: Carte (o vuoto) + Spaziatore (variabile) + Box
    return leftPart + spacer + rightPart;
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
