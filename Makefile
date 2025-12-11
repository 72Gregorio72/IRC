NAME		= ircserv

SRCS		=   main.cpp Server.cpp \
				User.cpp Channel.cpp \
				ServerErrorMessage.cpp \
				ServerLoop.cpp \
				ServerParsing.cpp \
				ServerReplyMessage.cpp \
				BalatroBot/Balatro.cpp \
				BalatroBot/Card.cpp \
				BalatroBot/BalatroUI.cpp \
				BalatroBot/BalatroSelectedUI.cpp \
				BalatroBot/PokerHand.cpp \

OBJS		= $(SRCS:.cpp=.o)

RM			= rm -f
FLAGS		= -Wall -Wextra -Werror -std=c++98
COMPILER	= c++

.cpp.o:
	${COMPILER} ${FLAGS} -c $< -o ${<:.cpp=.o}

all: $(NAME)

$(NAME): ${OBJS}
	${COMPILER} ${FLAGS} $(OBJS) -o $(NAME)
	@echo "$(GREEN)[$(NAME)]:\t PROJECT COMPILED$(RESET)"

clean:
	$(RM) -f $(OBJS)
	@echo "$(RED)[$(NAME)]:\t CLEAN$(RESET)"

fclean: clean
	rm -f $(NAME)
	@echo "$(RED)[$(NAME)]:\t FCLEAN$(RESET)"

re: fclean all

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
	@./$(NAME) 7272 1234

#COLORS

GREEN=\033[0;32m
RED=\033[0;31m
BLUE=\033[0;34m
RESET=\033[0m
