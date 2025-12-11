#include "Balatro.hpp"
#include <sstream>

// Helper per gli spazi
std::string Balatro::getSpaces(int count) {
    if (count < 0) return "";
    return std::string(count, ' ');
}

// Funzione Unificata: Gestisce sia carte normali che selezionate
// Se isSelected = true, la carta viene stampata "in alto" (senza padding sopra)
// Se isSelected = false, la carta viene stampata "in basso" (con padding sopra)
std::vector<std::string> Balatro::getCardRowsSelected(const Card& c, bool isSelected) {
    std::vector<std::string> rows;
    std::string rank = c.getRank();
    std::string suit = c.getSuit();
    
    // Definizioni colori
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

    // Pattern grafici
    std::string mid = "     " + sym + "     ";
    std::string col = "  " + sym + "     " + sym + "  ";
    std::string midCol = "  " + sym + "  " + sym + "  " + sym + "  ";

    // Costruzione delle 5 righe interne
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
            line[0] = col; line[1] = col; line[2] = mid; line[3] = col; line[4] = col;
        }
        else if (r == 10) {
            line[0] = col; line[1] = midCol; line[2] = mid; line[3] = col; line[4] = col;
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

    // Costruiamo le 9 righe della grafica della carta
    std::vector<std::string> cardGraphic;
    cardGraphic.push_back(color + "┌───────────┐" + RESET);      
    cardGraphic.push_back(color + "│ " + rL + "        │" + RESET); 
    cardGraphic.push_back(color + "│" + line[0] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[1] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[2] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[3] + "│" + RESET);    
    cardGraphic.push_back(color + "│" + line[4] + "│" + RESET);    
    cardGraphic.push_back(color + "│        " + rR + " │" + RESET); 
    cardGraphic.push_back(color + "└───────────┘" + RESET);

    // --- LOGICA DI SPOSTAMENTO VERTICALE (PADDING) ---
    // La larghezza di una carta è 13 caratteri
    std::string emptyRow = "             "; 

    // Selezionata: Carta in alto (0-8), Spazio vuoto sotto (9-10)
    if (isSelected) {
        for(size_t i = 0; i < cardGraphic.size(); i++) rows.push_back(cardGraphic[i]);
        rows.push_back(emptyRow);
        rows.push_back(emptyRow);
    } 
    // Non Selezionata: Spazio vuoto sopra (0-1), Carta in basso (2-10)
    else {
        rows.push_back(emptyRow);
        rows.push_back(emptyRow);
        for(size_t i = 0; i < cardGraphic.size(); i++) rows.push_back(cardGraphic[i]);
    }

    return rows; // Ritorna sempre 11 righe
}

void Balatro::printSelectedCardsUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player.getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29; 

    // 1. Preparazione grafica carte (con logica selezione integrata)
    std::vector< std::vector<std::string> > cardMatrix;

	if (selectedCards.empty()) {
		// Se non ci sono carte selezionate, stampiamo la UI normale
		printUI();
		return;
	}
    
    for(size_t i = 0; i < hand.size(); i++) {
        // Controlla se l'indice 'i' è presente nel vettore selectedCards
        bool isSelected = false;
		for (size_t j = 0; j < selectedCards.size(); j++) {
			if (hand[i].getRank() == selectedCards[j].getRank() &&
				hand[i].getSuit() == selectedCards[j].getSuit()) {
				isSelected = true;
				break;
			}
		}
        // Passiamo isSelected alla funzione: la matrice avrà carte spostate visivamente
        cardMatrix.push_back(getCardRowsSelected(hand[i], isSelected));
    }

    std::vector<std::string> deckVisual = printDeck();
    
    // 2. Calcolo altezze e offset
    // Nota: handHeight ora sarà 11 (9 + 2 di padding)
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

template <typename T>
std::string to_string_98(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// Helper per ripetere stringhe (simil-costruttore string(count, char))
std::string repeat_char(int count, char c) {
    if (count <= 0) return "";
    return std::string(count, c);
}

struct RowData {
	std::string label;
	int val;
	std::string color;
	int hiddenColorLen; // Lunghezza dei codici colore nella label per correggere l'allineamento
};

void Balatro::printEndRoundUI() {
    std::string prefix = ":BalatroBot PRIVMSG " + player.getNickName() + " :";
    
    // Dimensioni
    int totalRows = 58;
    int leftColWidth = 29;
    
    // Definizioni colori IRC
    std::string RED = "\x03" "04";
    std::string ORANGE = "\x03" "07";
    std::string GREEN = "\x03" "09";  // Verde chiaro
    std::string BLUE = "\x03" "12";
    std::string GREY = "\x03" "14";   // Grigio scuro
    std::string WHITE = "\x03" "00";
    std::string RESET = "\x0f";

    // --- 1. DATI PLACEHOLDER ---
    long blindScoreReq = 10000;
    int blindReward = 0; 
    
    int handsLeft = 3;
    int moneyPerHand = 1;
    int handsReward = handsLeft * moneyPerHand;
    
    //int discardsLeft = 2; 
    int interest = 0; 
    int bossBonus = 0;
    
    int totalCashOut = blindReward + handsReward + interest + bossBonus;

    // --- 2. COSTRUZIONE DEL BOX "CASH OUT" ---
    std::vector<std::string> boxLines;
    int boxContentWidth = 36; 

    // Header del Box
    std::string sTotal = to_string_98(totalCashOut);
    std::string headerTitle = " Cash Out: $" + sTotal + " ";
    
    // Calcolo padding header per centrare
    int padHead = (boxContentWidth - (int)headerTitle.length()) / 2;
    if (padHead < 0) padHead = 0;
    
    std::string headerSpace = repeat_char(padHead, ' ');
    // Bottone Arancione
    std::string btn = "\x03" "00,07" + headerSpace + headerTitle + headerSpace + ((headerTitle.length() % 2 != 0) ? " " : "") + RESET;

    boxLines.push_back(GREY + "╭──────────────────────────────────────╮" + RESET);
    boxLines.push_back(GREY + "│" + RESET + " " + btn + " " + GREY + "│" + RESET);
    boxLines.push_back(GREY + "├──────────────────────────────────────┤" + RESET);
    
    // Per C++98 dobbiamo costruire le righe manualmente senza lambda.
    // Struttura dati temporanea per le righe: Label, Valore, ColoreValore, LunghezzaExtra(colori)

    std::vector<RowData> rowsToPrint;
    
    // Riga 1: Score
    RowData r1; 
    r1.label = "Score at least " + RED + to_string_98(blindScoreReq) + RESET;
    r1.val = blindReward; 
    r1.color = GREEN;
    r1.hiddenColorLen = RED.length() + RESET.length(); 
    rowsToPrint.push_back(r1);

    // Riga 2: Hands
    RowData r2;
    r2.label = BLUE + to_string_98(handsLeft) + RESET + " Remaining Hands ($1)";
    r2.val = handsReward;
    r2.color = GREEN;
    r2.hiddenColorLen = BLUE.length() + RESET.length();
    rowsToPrint.push_back(r2);

    // Riga 3: Interest
    RowData r3;
    r3.label = "Interest (max 5)";
    r3.val = interest;
    r3.color = GREEN;
    r3.hiddenColorLen = 0;
    rowsToPrint.push_back(r3);

    // Riga 4: Boss (opzionale)
    if (bossBonus > 0) {
        RowData r4;
        r4.label = "Boss Blind Defeated";
        r4.val = bossBonus;
        r4.color = GREEN;
        r4.hiddenColorLen = 0;
        rowsToPrint.push_back(r4);
    } else {
        // Riga vuota placeholder per mantenere l'altezza
        RowData rEmpty;
        rEmpty.label = ""; 
        rEmpty.val = -1; // Flag per dire "vuoto"
        rowsToPrint.push_back(rEmpty);
    }

    // Processamento righe
    for(size_t i = 0; i < rowsToPrint.size(); ++i) {
        if (rowsToPrint[i].val == -1) {
            // Riga vuota
            boxLines.push_back(GREY + "│" + RESET + repeat_char(boxContentWidth + 2, ' ') + GREY + "│" + RESET);
            continue;
        }

        std::string sVal = "$" + to_string_98(rowsToPrint[i].val);
        
        // Calcolo puntini: Lunghezza scatola - (Label visibile + Valore + spazi)
        int visibleLabelLen = (int)rowsToPrint[i].label.length() - rowsToPrint[i].hiddenColorLen;
        int dots = boxContentWidth - (visibleLabelLen + (int)sVal.length() + 2);
        if (dots < 0) dots = 0;

        std::string dottedLine = repeat_char(dots, '.');
        
        std::string finalLine = rowsToPrint[i].label + " " + GREY + dottedLine + RESET + " " + rowsToPrint[i].color + sVal + RESET;
        boxLines.push_back(GREY + "│" + RESET + " " + finalLine + " " + GREY + "│" + RESET);
    }

    boxLines.push_back(GREY + "╰──────────────────────────────────────╯" + RESET);


    // --- 3. RENDERING ---
    int boxHeight = (int)boxLines.size();
    int startBoxRow = (totalRows / 2) - (boxHeight / 2);

    std::string msg = "";
    
    // Header UI
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════\r\n";

    for (int row = 0; row < totalRows; ++row) {
        // A. Pannello Sinistro
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        
        std::string padding = repeat_char(leftColWidth - (int)leftRaw.length(), ' ');
        std::string leftPanel = leftColor + padding;

        // B. Pannello Destro
        std::string rightPanel = "";
        
        if (row >= startBoxRow && row < startBoxRow + boxHeight) {
            int boxIdx = row - startBoxRow;
            // Padding fisso a sinistra per centrare il box nell'area destra
            rightPanel = "             " + boxLines[boxIdx]; 
        } 
        
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }

    // Footer UI
    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════\r\n";
    
    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}