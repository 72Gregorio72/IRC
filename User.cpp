#include "User.hpp"

User::User() : User_name(""), isOp(false) {}

User::User(std::string nickname, std::string username, std::string hostname, std::string servername, std::string realname)
: Nick_name(nickname), User_name(username), Host_name(hostname), Server_name(servername), Real_name(realname), isOp(false) {}

User::~User() {}

User::User(const User &other) : User_name(other.User_name), isOp(other.isOp) {}

User& User::operator=(const User &other){
	if (this == &other)
		return *this;
	User_name = other.User_name;
	isOp = other.isOp;
	return *this;
}

