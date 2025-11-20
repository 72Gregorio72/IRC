#ifndef SERVER_HPP
#define SERVER_HPP

#include "ft_irc.h"

struct data{
	int socket_fd, port_number, accept_fd;
	struct sockaddr_in server_sock, client_sock;
	char msg[256];
	ssize_t return_value;
	int max_sd;
	fd_set master_fd;
	fd_set read_fd;
} typedef data;

class Server{
	private:
		std::string password;
		data serverdata;
	public:
		Server();
		~Server();
		Server(std::string pass);
		Server(const Server &other);
		Server &operator=(const Server &other);
		std::string getPassword();
		void	open_server(char **av);
		void	server_loop();
		void	parse_msg();
		bool	check_password(std::string password);
		void	close_all();
};

#endif