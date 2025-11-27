#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.h"

class User;

class Channel{
	public:
		Channel();
		Channel(std::string name);
		~Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

		std::string getChannelName();
		std::vector<User> getUsers();
		void addUser(User user);
		void removeUser(std::string nickname);

		Channel *findChannelByName(std::string name);
	private:
		std::string channel_name;
		std::vector <User> users;
		std::vector<Channel> allChannels;
		std::string topic;
};

#endif