#include "ft_irc.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


int main(int ac, char *av[])
{
	if (ac != 3) {
		fprintf(stderr,"ERROR: Usage: <port> <password>\n");
		exit(1);
	}
	Server server = Server(av[2]);
	server.open_server(av);
	server.server_loop();
	server.close_all();
	return 0; 
}