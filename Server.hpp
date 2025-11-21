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

class User;

class Server{

	public:

		Server();
		~Server();
		Server(std::string pass);
		Server(const Server &other);
		Server &operator=(const Server &other);
		
		void	open_server(char **av);
		void	server_loop();
		int		parse_msg(int sd);
		bool	check_password(std::string password);
		void	close_all();
		User	*find_by_sd(int sd);

		std::vector <User *>	getUsers();
		std::string 		getPassword();

		void	add_user(User *newUser);
		void	create_user(std::string msg, int sd);
		void	remove_user(int sd);
		void	print_users();

	private:

		std::string password;
		data serverdata;
		std::vector <User *> users;
};

#endif