#ifndef USER_HPP
#define USER_HPP

#include "ft_irc.h"

class User{
	public:
		User();
		User(std::string nickname, std::string username, std::string hostname, std::string servername, std::string realname);
		~User();
		std::string getNickName();
		User(const User &other);
		User &operator=(const User &other);
	private:
		std::string Nick_name;
		std::string User_name;
		std::string Host_name;
		std::string Server_name;
		std::string Real_name;
		bool isOp;
};

#endif