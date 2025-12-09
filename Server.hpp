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
class Channel;
class Balatro;

class Server{

	public:

		Server();
		~Server();
		Server(std::string pass);
		Server(const Server &other);
		Server &operator=(const Server &other);
		
		void	open_server(char **av);

		void	server_loop();
		void	handle_new_connection();
		void	handle_client_read(int sd);
		int		process_user_buffer(User *user, int sd);

		int		parse_msg(int sd);
		bool	check_password(std::string password);
		void	close_all();
		User	*find_by_sd(int sd);
		User	*find_by_nickname(std::string nickname);

		std::vector <User *>	getUsers();
		std::string 		getPassword();

		void	add_user(User *newUser);
		void	create_user(std::string msg, int sd);
		void	remove_user(int sd);
		void	print_users();

		void	replyErrToClient(int numErrno, std::string nickname, std::string channel, int sd, std::string arg);
		void    replyServToClient(int numErrno, std::string nickname, int sd, std::string channelName, std::string addMsg);


		void deleteChannel(std::string channelName);

		void	sendPrivmsg(std::string msg, User* sender);

		std::string getServerName();

		Channel* findChannelByName(std::string name);

		int kick(std::string msg, int sd);
		int part(std::string msg, int sd);
		int parse_entry(std::string msg, int sd);
		int	topic(std::string msg, int sd);
		int	invite(std::string msg, int sd);

		bool assignPasswordToChannel(std::string channels, std::string passwords, Channel *channel);
		bool checkUserPassword(Channel *channel, std::string channelsCopy, std::string passwords);
		int parse_join(std::string msg);
		int mode(std::string msg, int sd);
		bool alreadyInChannel(std::string nickname, std::string channelName);

		Balatro* findBalatroBySd(int sd);

		void addBalatroBot(int sd, User player);
	private:
		std::vector<Channel> allChannels;
		std::string password;
		data serverdata;
		std::vector <User *> users;
		std::string	serverName;
		std::vector<Balatro> balatroBots;
};

#endif