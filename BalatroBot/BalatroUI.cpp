#include "includes/Balatro.hpp"

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::vector<std::string> Balatro::getCombinedJokersVisual(const std::vector<IJoker*>& targetJokers) {
    if (targetJokers.empty()) {
        return std::vector<std::string>();
    }

    // 1. Genera i singoli box
    std::vector< std::vector<std::string> > jokerImages;
    for (size_t i = 0; i < targetJokers.size(); ++i) {
        jokerImages.push_back(createJokerItem(targetJokers[i]));
    }

    if (jokerImages.empty()) return std::vector<std::string>();

    // 2. Unisci orizzontalmente
    int itemsHeight = (int)jokerImages[0].size();
    int gapSize = 1; 
    std::string spacer = std::string(gapSize, ' ');

    std::vector<std::string> mergedRow(itemsHeight, "");

    for (int r = 0; r < itemsHeight; ++r) {
        for (size_t i = 0; i < jokerImages.size(); ++i) {
            mergedRow[r] += jokerImages[i][r];
            if (i < jokerImages.size() - 1) {
                mergedRow[r] += spacer;
            }
        }
    }
    return mergedRow;
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
	std::string RED = "\x03" "04";
    std::string LIGHT_BLUE = "\x03" "12";
    std::string BLACK = "\x03" "01";
	std::string LINE_BG = "\x03" "01,04";
	std::string WHITE_ON_RED = "\x03" "00,04";
    std::string TEXTURE = "╬";
    std::string RESET = "\x0f";
    std::string BLOCK = "█";

    int width = 18; 
    int height = 12;

    // double cx = (width - 1) / 2.0;
    // double cy = (height - 1) / 2.0;
    
    // double rx = 5.0; 
    // double ry = 3.5; 

    rows.push_back(BLACK + "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" + RESET);

    // for (int y = 0; y < height; ++y) {
    //     std::string line = "";
    //     for (int x = 0; x < width; ++x) {
    //         double dx = x - cx;
    //         double dy = y - cy;
    //         double dist = (dx * dx) / (rx * rx) + (dy * dy) / (ry * ry);

    //         if (x == 2 && y == 1) {
    //             line += LINE_BG + "╠";
    //         }
    //         else if (x == width - 3 && y == 1) {
    //             line += LINE_BG + "╦";
    //         }
    //         else if (x == 2 && y == height - 2) {
    //             line += LINE_BG + "╩";
    //         }
    //         else if (x == width - 3 && y == height - 2) {
    //             line += LINE_BG + "╣";
    //         }
    //         else if (x == 2 && y <= height - 2) {
    //             line += LINE_BG + "║";
    //         }
    //         else if (x == width - 3 && y >= 1) {
    //             line += LINE_BG + "║";
    //         }
    //         else if (y == 1 && x >= 2) {
    //             line += LINE_BG + "═";
    //         }
    //         else if (y == height - 2 && x <= width - 3) {
    //             line += LINE_BG + "═";
    //         }
    //         else {
    //             if (dist <= 1.0) {
    //                 line += RESET + BLACK + BLOCK;
    //             } else {
    //                 line += RESET + RED + BLOCK;
    //             }
    //         }
    //     }
    //     rows.push_back(BLACK + "█" + line + BLACK + "█" + RESET);
    // }

	for (int y = 0; y < height; ++y) {
        std::string line = "";
        for (int x = 0; x < width; ++x) {
            // Applica il carattere ╬ con linee bianche e sfondo rosso su ogni cella
            line += WHITE_ON_RED + TEXTURE;
        }
        // Aggiunge i bordi laterali neri attorno alla linea generata
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
    int boxWidth = 44;          // Larghezza box comandi
    int targetRightCol = 85;    // Dove inizia visivamente il box comandi
    int cardWidth = 14;         

    // --- CALCOLO SPAZIATURA ---
    // Calcoliamo dove finiscono le carte per sapere quanti spazi mettere prima del box destro
    int currentHandWidth = hand.size() * cardWidth;
    int boxStartCol = targetRightCol;
    
    // Se la mano è troppo larga, sposta il box a destra
    if (currentHandWidth + 4 > targetRightCol) {
        boxStartCol = currentHandWidth + 4;
    }

    // --- COLORI ---
    std::string C_ORANGE  = "\x03" "07";
    // Scala da Blu (Freddo) a Rosso (Caldo)
    std::string C_SCALE_1 = "\x03" "02"; // Navy Blue
    std::string C_SCALE_2 = "\x03" "12"; // Blue
    std::string C_SCALE_3 = "\x03" "11"; // Cyan
    std::string C_SCALE_4 = "\x03" "10"; // Teal
    std::string C_SCALE_5 = "\x03" "09"; // Light Green
    std::string C_SCALE_6 = "\x03" "08"; // Yellow
    std::string C_SCALE_7 = "\x03" "07"; // Orange
    std::string C_SCALE_8 = "\x03" "04"; // Red
    std::string C_SCALE_9 = "\x03" "05"; // Maroon (Dark Red)
    std::string C_SCALE_10 = "\x03" "06";
    std::string C_GREY    = "\x03" "14";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // --- 1. CONTENUTO "SINISTRO" (CARTE DELLA MANO) ---
    std::string leftPart = "";
    int visibleLeftLen = 0; 

    if (row >= handStart && row < handStart + handH) {
        int slice = row - handStart;
        for(size_t c = 0; c < cardMatrix.size(); c++) {
            leftPart += cardMatrix[c][slice] + " ";
        }
        visibleLeftLen = currentHandWidth;
    }
    else if (row == handStart + handH) {
        for(size_t c = 0; c < hand.size(); c++) {
            std::string cardNum = "(" + toString(c + 1) + ")";
            int paddingLen = (13 - cardNum.length()) / 2;
            std::string item = std::string(paddingLen, ' ') + cardNum + std::string(13 - paddingLen - cardNum.length(), ' ') + " ";
            leftPart += item;
        }
        visibleLeftLen = currentHandWidth;
    }

    // --- 2. SPAZIATURA CENTRALE ---
    int paddingNeeded = boxStartCol - visibleLeftLen;
    if (paddingNeeded < 0) paddingNeeded = 0;
    std::string spacer = std::string(paddingNeeded, ' ');

    // --- 3. CONTENUTO DESTRO (BOX COMANDI + DECK) ---
    std::string rightPart = "";
    std::string vBorder = C_ORANGE + "│" + RESET; 

    // Fissiamo l'inizio del box comandi in alto (es. riga 2)
    int boxCommandStartRow = 2; 

    // A. HEADER BOX
    if (row == 0) {
        rightPart = C_ORANGE + "┌──────────────────────────────────────────┐" + RESET;
    }
    // B. SEZIONE COMANDI
    else if (row >= boxCommandStartRow && row < deckStart) {
        
        int r = row - boxCommandStartRow + 1; // Riga relativa

        if (row == deckStart - 1) {
             rightPart = C_ORANGE + "└──────────────────────────────────────────┘" + RESET;
        }
        else {
            std::string text = "";
            if (r == 1)      text = BOLD + centerText("COMMANDS", boxWidth - 2) + RESET;
            else if (r == 2) text = C_ORANGE + "├──────────────────────────────────────────┤" + RESET;
            else if (r == 3) text = " " + C_SCALE_1 + "!select" + RESET + " <id> " + C_GREY + "(es: !select 1 3)" + RESET;
            else if (r == 5) text = " " + C_SCALE_2 + "!discard" + C_GREY + " (Discard selected)" + RESET;
            else if (r == 7) text = " " + C_SCALE_3 + "!play" + C_GREY + "    (Play selected)" + RESET;
            else if (r == 9) text = " " + C_SCALE_4 + "!sort suit/rank" + C_GREY + " (Sort Hand)" + RESET;
            else if (r == 11) text = " " + C_SCALE_5 + "!cash out" + C_GREY + " (Cash Out)" + RESET;
            else if (r == 13) text = " " + C_SCALE_6 + "!shop"  + RESET + " <id> " + C_GREY + "(es: !shop 1 3)" + RESET;
            else if (r == 15) text = " " + C_SCALE_7 + "!next" + C_GREY + " (Start New Round)" + RESET;
            else if (r == 17) text = " " + C_SCALE_8 + "!replace"  + RESET + " <id> " + C_GREY + "(es: !replace 1 3)" + RESET;
            else if (r == 19) text = " " + C_SCALE_9 + "!reroll" + C_GREY + " (Reroll The Shop)" + RESET;
            else if (r == 21) text = " " + C_SCALE_10 + "!sell"  + RESET + " <id> " + C_GREY + "(es: !sell 1 3)" + RESET;
            else {
                text = std::string(boxWidth - 2, ' '); 
            }

            if (r == 2) {
                rightPart = text;
            } else {
                 int txtLen = getVisualLength(text);
                 if (txtLen > boxWidth - 2) text = std::string(boxWidth - 2, ' '); 
                 int padRight = (boxWidth - 2) - txtLen;
                 if(padRight < 0) padRight = 0;
                 rightPart = vBorder + text + std::string(padRight, ' ') + vBorder;
            }
        }
    }
    // C. RIEMPIMENTO VERTICALE (Tra header e box comandi, se c'è spazio)
    else if (row < deckStart && row > 0) {
        rightPart = vBorder + std::string(boxWidth - 2, ' ') + vBorder;
    }
    // D. SEZIONE DECK
    else if (row >= deckStart && row < deckStart + deckH) {
        int dSlice = row - deckStart;
        std::string deckRow = deckVisual[dSlice];
        int deckPadLen = (boxWidth - 20) / 2;
        if (deckPadLen < 0) deckPadLen = 0;
        rightPart = std::string(deckPadLen, ' ') + deckRow;
    }
    else if (row == deckStart + deckH) {
        std::string deckLabel = "(" + toString(deck.size()) + ")";
        int labelPadLen = (boxWidth - deckLabel.length()) / 2;
        rightPart = std::string(labelPadLen, ' ') + deckLabel; 
    }

    return leftPart + spacer + rightPart;
}

void Balatro::printUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    std::string C_RED     = "\x03" "04";
    std::string C_WHITE   = "\x03" "00";
    std::string C_YELLOW  = "\x03" "08";
    std::string BOLD      = "\x02";
    std::string RESET     = "\x0f";

    // --- GAME OVER CHECK ---
    if (gameOver) {
        std::string msg = "";
        // Pulizia schermo
        for(int i=0; i<50; i++) msg += prefix + " \r\n";

        std::vector<std::string> art;
        art.push_back("  /$$$$$$   /$$$$$$  /$$      /$$ /$$$$$$$$        /$$$$$$  /$$    /$$ /$$$$$$$$ /$$$$$$$   ");
        art.push_back(" /$$__  $$ /$$__  $$| $$$    /$$$| $$_____/       /$$__  $$| $$   | $$| $$_____/| $$__  $$  ");
        art.push_back("| $$  \\__/| $$  \\ $$| $$$$  /$$$$| $$            | $$  \\ $$| $$   | $$| $$      | $$  \\ $$  ");
        art.push_back("| $$ /$$$$| $$$$$$$$| $$ $$/$$ $$| $$$$$         | $$  | $$|  $$ / $$/| $$$$$   | $$$$$$$/  ");
        art.push_back("| $$|_  $$| $$__  $$| $$  $$$| $$| $$__/         | $$  | $$ \\  $$ $$/ | $$__/   | $$__  $$  ");
        art.push_back("| $$  \\ $$| $$  | $$| $$ \\ $ | $$| $$            | $$  | $$  \\  $$$/  | $$      | $$  \\ $$  ");
        art.push_back("|  $$$$$$/| $$  | $$| $$ \\/  | $$| $$$$$$$$      |  $$$$$$/   \\  $/   | $$$$$$$$| $$  | $$  ");
        art.push_back(" \\______/ |__/  |__/|__/     |__/|________/       \\______/     \\_/    |________/|__/  |__/  ");

        for (size_t i = 0; i < art.size(); ++i) {
            std::string centeredLine = centerText(art[i], 100); 
            msg += prefix + C_RED + BOLD + centeredLine + RESET + "\r\n";
        }

        msg += prefix + " \r\n";
        msg += prefix + " \r\n";

        std::string scoreText = "FINAL SCORE: " + toString(totalBet); 
        std::string centeredScore = centerText(scoreText, 100);
        msg += prefix + C_WHITE + BOLD + centeredScore + RESET + "\r\n";

        std::string restartText = "Type /balatro to play again";
        std::string centeredRestart = centerText(restartText, 100);
        msg += prefix + C_YELLOW + centeredRestart + RESET + "\r\n";

        // Footer pulizia
        for(int i=0; i<20; i++) msg += prefix + " \r\n";

        send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
        return; 
    }

    // --- LOGICA DI GIOCO STANDARD ---

    int totalRows = 58;
    int leftColWidth = 29; 

    // 1. Preparazione Grafica Elementi
    // Carte
    std::vector< std::vector<std::string> > cardMatrix;
    for(size_t i = 0; i < hand.size(); i++) {
        cardMatrix.push_back(getCardRows(hand[i]));
    }
    // Mazzo
    std::vector<std::string> deckVisual = printDeck();
    // Joker (Grafica combinata)
    std::vector<std::string> jokersVisual = getCombinedJokersVisual(this->jokers); 
    
    // 2. Calcolo altezze e posizioni verticali
    int handHeight = (!cardMatrix.empty() && !cardMatrix[0].empty()) ? (int)cardMatrix[0].size() : 0;
    int deckHeight = (int)deckVisual.size();
    int jokerHeight = (int)jokersVisual.size();

    int bottomBase = totalRows - 3; 
    int handStartRow = bottomBase - handHeight;
    int deckStartRow = bottomBase - deckHeight;
    
    // Posizione verticale dei Joker: Riga 2 (subito sotto la linea di header)
    int jokerStartRow = 2; 

    std::string msg = "";

    // 3. Header Tavolo
    for(int i=0; i<10; i++) msg += prefix + " \r\n";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    // 4. Ciclo di Rendering (Riga per riga)
    for (int row = 0; row < totalRows; ++row) {
        
        // --- Pannello Sinistro (Stats) ---
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        int spacesNeeded = leftColWidth - (int)leftRaw.length();
        if (spacesNeeded < 0) spacesNeeded = 0;
        std::string leftPanel = leftColor + std::string(spacesNeeded, ' ');

        // --- Pannello Destro Base (Senza Joker) ---
        // Contiene la mano (in basso) e il box comandi/deck (a destra)
        std::string rightPanel = getRightPanelContent(row, handStartRow, handHeight, deckStartRow, deckHeight, cardMatrix, deckVisual);
        
        // --- INIEZIONE JOKER NEL CENTRO ---
        // Se siamo nelle righe dedicate ai joker...
        if (row >= jokerStartRow && row < jokerStartRow + jokerHeight) {
            // Ottieni la riga grafica del joker corrente
            std::string jLine = jokersVisual[row - jokerStartRow];
            
            // Calcolo posizione orizzontale
            // Il Box Comandi inizia circa a colonna 85.
            // Vogliamo centrare i joker nello spazio disponibile (0 -> 85). Centro = ~42.
            int centerPos = 42;
            int visualLen = getVisualLength(jLine);
            int startPos = centerPos - (visualLen / 2);
            
            if (startPos < 2) startPos = 2; // Margine minimo sinistro

            // Controllo sovrapposizione: Assicuriamoci che rightPanel sia abbastanza lunga
            // e che non stiamo scrivendo sopra il box comandi
            if (startPos + visualLen < (int)rightPanel.length()) {
                // Sovrascriviamo gli spazi vuoti con la grafica del joker.
                // Nota: replace accetta la lunghezza in bytes, ma noi stiamo rimpiazzando spazi (1 byte = 1 char).
                // Quindi rimuoviamo 'visualLen' spazi e inseriamo 'jLine'.
                rightPanel.replace(startPos, visualLen, jLine);
            }
        }

        // Assemblaggio messaggio IRC
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }

    // 5. Footer Tavolo
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";

    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}