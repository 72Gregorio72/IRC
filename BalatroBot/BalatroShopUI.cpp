#include "includes/Balatro.hpp"

std::string repeat_string(int count, const std::string& pattern) {
    std::string result = "";
    for (int i = 0; i < count; ++i) {
        result += pattern;
    }
    return result;
}


int getVisualLength(const std::string& s) {
    int visualLen = 0;
    //bool inColor = false; // Flag per tracking stato colore (opzionale, qui gestiamo per char)

    for (size_t i = 0; i < s.length(); ++i) {
        unsigned char c = (unsigned char)s[i];

        // 1. Gestione Colori IRC (\x03)
        if (c == '\x03') {
            // Salta le cifre del primo colore (foreground)
            while (i + 1 < s.length() && isdigit(s[i+1])) { i++; }
            // Se c'è una virgola, controlla se seguono cifre (background)
            if (i + 1 < s.length() && s[i+1] == ',') {
                size_t j = i + 1; // Peek ahead
                if (j + 1 < s.length() && isdigit(s[j+1])) {
                    i++; // Consuma la virgola
                    while (i + 1 < s.length() && isdigit(s[i+1])) { i++; } // Consuma cifre bg
                }
            }
            continue; // Non contare questo carattere
        }

        // 2. Gestione altri controlli (Reset \x0f, Bold \x02, ecc.)
        // I char di controllo ASCII sono < 32. 
        if (c < 32) continue; 

        // 3. Gestione UTF-8
        // Byte di continuazione (10xxxxxx) non contano come colonna visiva
        if ((c & 0xC0) == 0x80) {
            continue;
        }

        // Se siamo qui, è un carattere visibile (ASCII o Inizio UTF-8)
        visualLen++;
    }
    return visualLen;
}

void Balatro::generateShopJokers() {
    // 1. Pulisce il negozio precedente (rimuove solo i puntatori, non fa delete sugli oggetti!)
    shopJokers.clear(); 
    
    // 2. Assicura che il database dei joker sia inizializzato
    initAllJokers(); 

    if (allJokers.empty()) return;

    // 3. Crea un vettore di indici [0, 1, 2, ... size-1]
    std::vector<int> indices;
    for (size_t i = 0; i < allJokers.size(); ++i) {
        indices.push_back(i);
    }

    // 4. Mescola gli indici casualmente
    std::random_shuffle(indices.begin(), indices.end());

    // 5. Seleziona i primi 4 (o meno se allJokers ne ha meno di 4)
    int maxShopItems = 4;
    int count = (allJokers.size() < (size_t)maxShopItems) ? allJokers.size() : maxShopItems;
    
    for (int i = 0; i < count; ++i) {
        shopJokers.push_back(allJokers[indices[i]]);
    }
}

std::vector<std::string> Balatro::createJokerItem(IJoker* joker) {
    std::vector<std::string> box;
    
    std::string RESET = "\x0f";
    std::string GREY  = "\x03" "14";
    std::string WHITE = "\x03" "00";
    std::string GREEN = "\x03" "09"; // Colore soldi
    std::string RED   = "\x03" "04"; // Colore nome Joker

    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";

    // Recupera dati reali dal Joker
    std::string name = joker->getName();
    std::string cost = "$" + to_string_98(joker->getCost());

    // Larghezza fissa interna (12 spazi per far stare nomi medi)
    int width = 12;

    // Se il nome è troppo lungo, lo tronchiamo
    if (name.length() > (size_t)width) {
        name = name.substr(0, width);
    }

    // 1. Bordo Superiore
    box.push_back(GREY + TL + repeat_string(width, H) + TR + RESET);

    // 2. Nome del Joker (Centrato)
    int vLenN = getVisualLength(name);
    int padN = (width - vLenN) / 2;
    if (padN < 0) padN = 0;
    
    box.push_back(GREY + V + RESET + 
                  repeat_char(padN, ' ') + RED + name + RESET + 
                  repeat_char(width - padN - vLenN, ' ') + 
                  GREY + V + RESET);

    // 3. Spazio vuoto decorativo (o label "JOKER")
    box.push_back(GREY + V + repeat_char(width, ' ') + V + RESET);

    // 4. Costo del Joker (Centrato)
    int vLenC = getVisualLength(cost);
    int padC = (width - vLenC) / 2;
    if (padC < 0) padC = 0;

    box.push_back(GREY + V + RESET + 
                  repeat_char(padC, ' ') + GREEN + cost + RESET + 
                  repeat_char(width - padC - vLenC, ' ') + 
                  GREY + V + RESET);

    // 5. Bordo Inferiore
    box.push_back(GREY + BL + repeat_string(width, H) + BR + RESET);

    return box;
}

template <typename T>
std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}


void Balatro::pasteObject(std::vector<std::string>& canvas, const std::vector<std::string>& object, int startRow, int startCol) {
    if (object.empty()) return;
    int objHeight = (int)object.size();

    for (int r = 0; r < objHeight; ++r) {
        int targetRow = startRow + r;
        if (targetRow >= (int)canvas.size()) break;

        std::string& line = canvas[targetRow];
        
        // Se la posizione di partenza è valida
        if (startCol < (int)line.length()) {
            std::string objLine = object[r];
            
            // 1. Prendi la parte PRIMA dell'oggetto
            std::string before = line.substr(0, startCol);
            
            // 2. Calcola quanto spazio visivo occupa l'oggetto che stiamo incollando
            int visualWidth = getVisualLength(objLine);
            if(visualWidth == 0) visualWidth = (int)objLine.length(); // Fallback

            // 3. Prendi la parte DOPO l'oggetto
            // Dobbiamo saltare 'visualWidth' caratteri dallo sfondo originale (che sono spazi)
            std::string after = "";
            
            // Nota: Questo assume che lo sfondo 'line' sia composto da spazi semplici nel punto in cui incolliamo.
            // Se incolli sopra testo colorato esistente, l'indice sarà sballato. 
            // Ma per il Canvas vuoto funziona perfettamente.
            if (startCol + visualWidth < (int)line.length()) {
                after = line.substr(startCol + visualWidth);
            }
            
            // Ricostruisci la linea
            line = before + objLine + after;
        }
    }
}

// Helper per ripetere una stringa (necessario perché '─' sono 3 byte, non un char)


// Crea un quadrato semplice per l'item
std::vector<std::string> Balatro::createSimpleItem(int id, int cost) {
    std::vector<std::string> box;
    std::string RESET = "\x0f";
    std::string GREEN = "\x03" "09"; 
    std::string GREY = "\x03" "14";
    std::string WHITE = "\x03" "00";
    
    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";

    std::string sId = to_string_98(id);
    std::string sCost = "$" + to_string_98(cost);
    std::string hLine = repeat_string(10, H);

    // Top
    box.push_back(GREY + TL + hLine + TR + RESET);
    
    // Nome: ITEM X
    std::string name = "ITEM " + sId;
    int vLenN = getVisualLength(name); 
    int padN = (10 - vLenN) / 2;
    if (padN < 0) padN = 0;
    
    box.push_back(GREY + V + RESET + repeat_char(padN, ' ') + WHITE + name + RESET + repeat_char(10 - padN - vLenN, ' ') + GREY + V + RESET);
    
    // Costo: $10
    int vLenC = getVisualLength(sCost);
    int padC = (10 - vLenC) / 2;
    if (padC < 0) padC = 0;

    box.push_back(GREY + V + RESET + repeat_char(padC, ' ') + GREEN + sCost + RESET + repeat_char(10 - padC - vLenC, ' ') + GREY + V + RESET);
    
    // Bottom
    box.push_back(GREY + BL + hLine + BR + RESET);
    
    return box;
}


// Crea un box per i messaggi (Next / Buy)
std::vector<std::string> Balatro::createMsgBox(std::string text, std::string colorCode){
    std::vector<std::string> box;
    std::string RESET = "\x0f";
    std::string GREY = "\x03" "14";

    std::string TL = "\xE2\x95\xAD"; std::string TR = "\xE2\x95\xAE";
    std::string BL = "\xE2\x95\xB0"; std::string BR = "\xE2\x95\xAF";
    std::string H  = "\xE2\x94\x80"; std::string V  = "\xE2\x94\x82";
    
    // Usa la lunghezza visiva per decidere la larghezza del box
    int vLen = getVisualLength(text);
    int innerWidth = vLen + 2; 
    
    std::string hLine = repeat_string(innerWidth, H);
    
    box.push_back(GREY + TL + hLine + TR + RESET);
    box.push_back(GREY + V + " " + RESET + colorCode + text + RESET + " " + GREY + V + RESET);
    box.push_back(GREY + BL + hLine + BR + RESET);
    
    return box;
}

// --- FUNZIONE PRINCIPALE ---
void Balatro::printShopUI() {
	isShopUI = true;
	coins = 1000;
    std::string prefix = ":BalatroBot PRIVMSG " + player->getNickName() + " :";
    
    int totalRows = 58;
    int leftColWidth = 29;
    int rightColWidth = 65; 
    
    std::string ORANGE = "\x03" "07";
    std::string BLUE = "\x03" "12";
    
    // 1. Canvas destro vuoto
    std::vector<std::string> rightCanvas(totalRows, repeat_char(rightColWidth, ' '));

    // 2. LOGICA JOKER SHOP
    // Se non ci sono joker nel negozio (es. nuovo round), generalic
    if (shopJokers.empty()) {
        generateShopJokers();
    }

    if (!shopJokers.empty()) {
        // Creiamo la grafica per i joker presenti nel vettore shopJokers
        std::vector< std::vector<std::string> > jokerImages;
        for (size_t i = 0; i < shopJokers.size(); ++i) {
            jokerImages.push_back(createJokerItem(shopJokers[i]));
        }

        // Parametri posizionamento
        int startY = 5;
        int startX = 2; // Un po' più a sinistra per farci stare 4 carte larghe
        int gapSize = 2; // Spazio tra le carte
        std::string spacer = repeat_char(gapSize, ' ');

        // Assumiamo che tutte le carte abbiano la stessa altezza
        int itemsHeight = (int)jokerImages[0].size(); 
        std::vector<std::string> mergedRow(itemsHeight, "");

        // Uniamo le immagini in una riga orizzontale
        for (int r = 0; r < itemsHeight; ++r) {
            for (size_t i = 0; i < jokerImages.size(); ++i) {
                mergedRow[r] += jokerImages[i][r];
                // Se non è l'ultimo, aggiungi spacer
                if (i < jokerImages.size() - 1) {
                    mergedRow[r] += spacer;
                }
            }
        }
        // Incolla la riga di carte sul canvas
        pasteObject(rightCanvas, mergedRow, startY, startX);
    } else {
        // Fallback se non ci sono joker (es. database vuoto)
        std::vector<std::string> msg = createMsgBox("SOLD OUT", ORANGE);
        pasteObject(rightCanvas, msg, 5, 4);
    }

    // 3. Comandi (Next / Buy)
    std::vector<std::string> nextBox = createMsgBox("!next to continue", ORANGE);
    pasteObject(rightCanvas, nextBox, 12, 22);

    // Nota: L'indice per comprare ora si riferisce all'indice visuale (1, 2, 3...)
    std::vector<std::string> buyBox = createMsgBox("!shop <1-" + to_string_98(shopJokers.size()) + "> to buy", BLUE);
    pasteObject(rightCanvas, buyBox, 17, 15);

    // 4. Stampa Finale
    std::string msg = "";
    msg += prefix + "═══════════════════════════════" + "╦" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    
    for (int row = 0; row < totalRows; ++row) {
        std::string leftRaw, leftColor;
        getLeftPanelContent(row, leftRaw, leftColor);
        
        int vLen = getVisualLength(leftRaw);
        int paddingNeeded = leftColWidth - vLen;
        if (paddingNeeded < 0) paddingNeeded = 0;

        std::string paddingL = repeat_char(paddingNeeded, ' ');
        std::string leftPanel = leftColor + paddingL;
        
        std::string rightPanel = rightCanvas[row];
        
        msg += prefix + " " + leftPanel + " ║ " + rightPanel + "\r\n";
    }

    msg += prefix + "═══════════════════════════════" + "╩" + "═════════════════════════════════════════════════════════════════";
    msg += prefix + "═════════════════════════════════════════════════════════════════════════════════════════════════════\r\n";
    send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}