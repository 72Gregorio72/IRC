#ifndef USER_HPP
#define USER_HPP

#include "ft_irc.h"

class User{
	public:

		std::string buffer;
		int sd;
		bool authenticated;

		User();
		User(int sd);
		User(std::string nickname, std::string username, std::string hostname, std::string servername, std::string realname);
		~User();
		User(const User &other);
		User &operator=(const User &other);


		std::string getNickName();
		std::string getUserName();
		std::string getHostName();
		std::string getServerName();
		std::string getRealName();

		void	setNickName(std::string nickname);

		void	printUser();

		void	setAll(std::string nickname, std::string username, std::string hostname, std::string servername, std::string realname);
		void 	setAll(std::string username, std::string hostname, std::string servername, std::string realname);
	
	private:
		std::string Nick_name;
		std::string User_name;
		std::string Host_name;
		std::string Server_name;
		std::string Real_name;
		bool isOp;
};

#endif