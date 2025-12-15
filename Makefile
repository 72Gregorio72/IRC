NAME		= ircserv

SRCS		=   main.cpp Server.cpp \
				User.cpp Channel.cpp \
				ServerErrorMessage.cpp \
				ServerLoop.cpp \
				ServerParsing.cpp \
				ServerReplyMessage.cpp \
				BalatroBot/Balatro.cpp \
				BalatroBot/BalatroGen.cpp \
				BalatroBot/Card.cpp \
				BalatroBot/BalatroUI.cpp \
				BalatroBot/BalatroSelectedUI.cpp \
				BalatroBot/PokerHand.cpp \
				BalatroBot/BalatroShopUI.cpp \
				BalatroBot/Jokers/BaseJoker/BaseJoker.cpp \
				BalatroBot/Jokers/CleverJoker/CleverJoker.cpp \
				BalatroBot/Jokers/CraftyJoker/CraftyJoker.cpp \
				BalatroBot/Jokers/CrazyJoker/CrazyJoker.cpp \
				BalatroBot/Jokers/DeviousJoker/DeviousJoker.cpp \
				BalatroBot/Jokers/DrollJoker/DrollJoker.cpp \
				BalatroBot/Jokers/GluttonousJoker/GluttonousJoker.cpp \
				BalatroBot/Jokers/GreedyJoker/GreedyJoker.cpp \
				BalatroBot/Jokers/JollyJoker/JollyJoker.cpp \
				BalatroBot/Jokers/LustyJoker/LustyJoker.cpp \
				BalatroBot/Jokers/MadJoker/MadJoker.cpp \
				BalatroBot/Jokers/SlyJoker/SlyJoker.cpp \
				BalatroBot/Jokers/WilyJoker/WilyJoker.cpp \
				BalatroBot/Jokers/WrathfulJoker/WrathfulJoker.cpp \
				BalatroBot/Jokers/ZanyJoker/ZanyJoker.cpp \
				BalatroBot/Jokers/TheDuoJoker/TheDuoJoker.cpp \
				BalatroBot/Jokers/TheTrioJoker/TheTrioJoker.cpp \
				BalatroBot/Jokers/TheFamilyJoker/TheFamilyJoker.cpp \
				BalatroBot/Jokers/TheOrderJoker/TheOrderJoker.cpp \
				BalatroBot/Jokers/TheTribeJoker/TheTribeJoker.cpp \

OBJS		= $(SRCS:.cpp=.o)

RM			= rm -f
FLAGS		= -Wall -Wextra -Werror -std=c++98
COMPILER	= c++

INCLUDES = -Iincludes

.cpp.o:
	$(COMPILER) $(FLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

# 2. Linker (crea l'eseguibile)
$(NAME): $(OBJS)
	$(COMPILER) $(FLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)[$(NAME)]:\t PROJECT COMPILED$(RESET)"

# --- Cleanup Rules ---

clean:
	$(RM) $(OBJS)
	@echo "$(RED)[$(NAME)]:\t CLEAN$(RESET)"

fclean: clean
	$(RM) $(NAME)
	@echo "$(RED)[$(NAME)]:\t FCLEAN$(RESET)"

re: fclean all

# --- Custom/Utility Rules ---

vale: re
	make clean
	@clear
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes ./$(NAME) 7272 1234

push: fclean
	if [ -d .vscode ]; then \
		rm -rf .vscode; \
	fi
	bash -i -c "push"

testosterone: re
	make clean
	@clear
	./$(NAME) 7272 1234

.PHONY: all clean fclean re vale push testosterone

#COLORS

GREEN=\033[0;32m
RED=\033[0;31m
BLUE=\033[0;34m
RESET=\033[0m
