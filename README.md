# IRC Server + BalatroBot

<img width="1917" height="1078" alt="Screenshot 2026-04-23 204230" src="https://github.com/user-attachments/assets/a3911369-e6b3-4528-8034-118227246696" />
<img width="1919" height="1079" alt="Screenshot 2026-04-23 204219" src="https://github.com/user-attachments/assets/2590e973-d910-4445-b3f8-ecad111be717" />


## Panoramica
Questo progetto implementa un server IRC in C++98 con gestione multi-client via socket non bloccanti e `select()`, piu una modalita gioco integrata chiamata BalatroBot.

Il server espone i comandi IRC principali richiesti per chat private e canali, oltre a una logica custom per avviare una partita testuale di Balatro direttamente tramite messaggi IRC.

## Caratteristiche principali
- Server TCP IRC con autenticazione iniziale (`PASS`, `NICK`, `USER`)
- Gestione canali con:
  - join/part
  - topic
  - invite
  - kick
  - mode canale (`+i`, `+t`, `+k`, `+o`, `+l` e rispettivi unset)
- Messaggistica privata (`PRIVMSG`) verso utenti o canali
- Chiusura pulita su `CTRL+C`
- Minigioco BalatroBot attivabile da client IRC

## Struttura del progetto
- Core IRC:
  - `main.cpp`
  - `Server.cpp`
  - `ServerLoop.cpp`
  - `ServerParsing.cpp`
  - `ServerReplyMessage.cpp`
  - `ServerErrorMessage.cpp`
  - `Channel.cpp`
  - `User.cpp`
- Header principali:
  - `includes/`
- BalatroBot e logica gioco:
  - `BalatroBot/`

## Requisiti
- Linux o ambiente compatibile POSIX (es. WSL Ubuntu)
- Compilatore C++ con supporto C++98 (es. `c++`/`g++`)
- `make`

## Build
Dalla root del progetto:

```bash
make
```

Viene generato l'eseguibile:

```bash
./ircserv
```

### Altri target utili
```bash
make clean
make fclean
make re
```

Nel `Makefile` sono presenti anche:
- `make test`: ricompila e avvia con porta/password di default
- `make valgrind`: esecuzione con valgrind

## Avvio server
Uso:

```bash
./ircserv <port> <password>
```

Esempio:

```bash
./ircserv 7272 1234
```

## Connessione da un client IRC
Puoi usare client GUI (HexChat) o CLI (Irssi/WeeChat). Una volta connesso al server, la sequenza minima e:

```text
PASS 1234
NICK tuoNick
USER tuoUser 0 * :Tuo Nome
```

Se la password e errata, il server rifiuta la sessione.

## Comandi IRC supportati
Comandi gestiti dal parser server:
- `PASS`
- `NICK`
- `USER`
- `JOIN`
- `PRIVMSG`
- `PART`
- `INVITE`
- `MODE`
- `TOPIC`
- `KICK`
- `QUIT`

Note:
- `JOIN` supporta anche join multipli separati da virgola e password canale.
- `MODE` canale include gestione operatori, invite-only, topic lock, chiave canale e user limit.

## BalatroBot: come usarlo
Il bot viene associato al tuo utente e risponde via `PRIVMSG`.

### Avvio partita
Dal client IRC invia:

```text
balatro
```

Per ricominciare una nuova partita/round quando richiesto:

```text
/balatro
```

### Invio comandi al bot
I comandi del bot passano come messaggi privati a `BalatroBot` e iniziano con `!`.

Comandi principali:
- `!select <id...>`: seleziona carte (max 5)
- `!discard`: scarta carte selezionate
- `!play`: gioca le carte selezionate
- `!sort suit`
- `!sort rank`
- `!cash out`
- `!reroll`
- `!next`
- `!shop <id...>`
- `!replace <id>`
- `!pack <1|2>`
- `!pick <id>`
- `!skip`
- `!sell <id...>`
- `!swap <id1> <id2>`

Suggerimento pratico:
1. Avvia round con `balatro`
2. Seleziona carte con `!select`
3. Gioca con `!play` o scarta con `!discard`
4. A fine round passa dallo shop (`!cash out`, `!shop`, `!next`)

## Esempio rapido end-to-end
1. Compila:

```bash
make
```

2. Avvia server:

```bash
./ircserv 7272 1234
```

3. Connetti un client IRC a `127.0.0.1:7272`

4. Registra utente:

```text
PASS 1234
NICK player1
USER player1 0 * :Player One
```

5. Crea ed entra in un canale:

```text
JOIN #test
```

6. Avvia BalatroBot:

```text
balatro
```

## Troubleshooting
- Errore bind/listen: controlla che la porta non sia gia occupata.
- Client disconnesso subito: verifica ordine e correttezza di `PASS`, `NICK`, `USER`.
- Nessuna risposta utile da BalatroBot: assicurati di inviare comandi con prefisso `!` quando sei in partita.

## Note sviluppo
- Standard compilazione: `-std=c++98`
- Warning policy: `-Wall -Wextra -Werror`
- Il server usa I/O non bloccante e ciclo eventi basato su `select()`.
