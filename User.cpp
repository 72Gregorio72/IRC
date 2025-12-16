#include "User.hpp"

User::User() : buffer(""), sd(-1), authenticated(false), Nick_name(""), User_name(""), Host_name(""), Server_name(""), Real_name(""), isOp(false){}

User::User(int sd) :  buffer(""), sd(sd), authenticated(false), Nick_name(""), User_name(""), Host_name(""), Server_name(""), Real_name(""), isOp(false){
}

User::User(std::string nickname, std::string username, std::string hostname, std::string servername, std::string realname)
: sd(-1), authenticated(false), Nick_name(nickname), User_name(username), Host_name(hostname), Server_name(servername), Real_name(realname),  isOp(false) {}

User::~User() { authenticated = false; }

User::User(const User &other){
	this->sd = other.sd;
	this->buffer = other.buffer;
	this->authenticated = other.authenticated;
	this->Nick_name = other.Nick_name;
	this->User_name = other.User_name;
	this->Host_name = other.Host_name;
	this->Server_name = other.Server_name;
	this->Real_name = other.Real_name;
	this->isOp = other.isOp;

}

void	User::setAll(std::string nickname, std::string username, std::string hostname, std::string servername, std::string realname){
	this->Nick_name = nickname;
	this->User_name = username;
	this->Host_name = hostname;
	this->Server_name = servername;
	this->Real_name = realname;
}

void	User::setAll(std::string username, std::string hostname, std::string servername, std::string realname){
	this->User_name = username;
	this->Host_name = hostname;
	this->Server_name = servername;
	this->Real_name = realname;
}

User& User::operator=(const User &other){
	if (this == &other)
		return *this;
	User_name = other.User_name;
	isOp = other.isOp;
	return *this;
}

std::string User::getNickName(){
	return Nick_name;
}

std::string User::getUserName(){
	return User_name;
}

std::string User::getHostName(){
	return Host_name;
}

std::string User::getServerName(){
	return Server_name;
}

std::string User::getRealName(){
	return Real_name;
}

void	User::setNickName(std::string nickname){
	this->Nick_name = nickname;
}

bool User::_isOp() {
	return isOp;
}

void User::SetOp(bool val) {
	isOp = val;
}

void	User::printUser(){
	std::cout << "Nickname: " << this->Nick_name << std::endl;
	std::cout << "Username: " << this->User_name << std::endl;
	std::cout << "Hostname: " << this->Host_name << std::endl;
	std::cout << "Servername: " << this->Server_name << std::endl;
	std::cout << "Realname: " << this->Real_name << std::endl;
	std::cout << "Op: " << this->isOp << std::endl;

}
