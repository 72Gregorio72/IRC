#include "ft_irc.h"

int main(int ac, char *av[])
{
	if (ac != 3) {
		std::cout << "Usage: ./ircserv <port> <password>" << std::endl;
		exit(1);
	}
	Server server = Server(av[2]);
	server.open_server(av);
	server.server_loop();
	server.close_all();
	return 0; 
}