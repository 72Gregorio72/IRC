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
		bool	getInviteOnly();
		std::string	getTopic();
		std::string getPassword();

		void addUser(User *user);
		int removeUser(std::string nickname);

		bool	userInChannel(std::string nickname);
		

		User *findUserByNickname(std::string nickname);
		User *findUserBySd(int sd);
		int count_operators();

		void	setTopic(std::string src_topic);
		void	setPassword(std::string password);

		void	setInviteOnly(bool value);
		void	setUserLimit(int limit);

		int getUserLimit();
	private:

		Server	*server;
		std::string channel_name;
		std::vector <User> users;
		std::string password;
		std::string topic;
		bool	inviteOnly;
		int		userLimit;
};

#endif