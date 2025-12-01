#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.h"

class User;
class Server;

class Channel{
	public:
		Channel();
		Channel(std::string name, Server *serv);
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		std::string getChannelName();
		std::vector<User> getUsers();
		void addUser(User *user);
		int removeUser(std::string nickname);

		bool	userInChannel(std::string nickname);
		

		User *findUserByNickname(std::string nickname);
		int count_operators();
	private:

		Server	*server;
		std::string channel_name;
		std::vector <User> users;
		std::string topic;
};

#endif