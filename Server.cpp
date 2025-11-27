#include "Server.hpp"
#include <string>

Server::Server() : password("1234"), serverName("Pranglost") {}

Server::~Server() {
    for (size_t i = 0; i < users.size(); i++) {
        delete users[i];
    }
    users.clear();
}

Server::Server(std::string pass) : password(pass), serverName("Pranglost") {}

Server::Server(const Server &other) : password(other.password), serverName(other.serverName) {}

Server& Server::operator=(const Server &other){
	if (this == &other)
		return *this;
	password = other.password;
	return *this;
}

std::string Server::getPassword(){
	return password;
}

std::string Server::getServerName(){
	return serverName;
}

void	Server::add_user(User *newUser){
	users.push_back(newUser);
}

void Server::remove_user(int sd){
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if ((*it)->sd == sd) {
            FD_CLR((*it)->sd, &serverdata.master_fd);
            delete *it;
            users.erase(it);
            break;
        }
    }
}

void	Server::print_users(){
	std::cout << "Current users:" << std::endl;
	for (int i = 0; i < (int)users.size(); ++i) {
		std::cout << "User " << i + 1 << ":" << std::endl;
		users[i]->printUser();
	}
}

std::vector <User *> Server::getUsers(){
	return users;
}

void	Server::open_server(char **av){
	serverdata.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
    if (setsockopt(serverdata.socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cout << "Error: setsockopt error" << std::endl;
        exit(1);
    }
	serverdata.port_number = std::atoi(av[1]);
	bzero((char *) &serverdata.server_sock, sizeof(serverdata.server_sock));
	serverdata.server_sock.sin_port = htons(serverdata.port_number);
	serverdata.server_sock.sin_family = AF_INET;
	serverdata.server_sock.sin_addr.s_addr = INADDR_ANY;
	if (bind(serverdata.socket_fd, (struct sockaddr *) &serverdata.server_sock, sizeof(serverdata.server_sock)) < 0){
		std::cout << "Error: bind error";
		exit(0);
	}
	if (listen(serverdata.socket_fd, 5) < 0){
		std::cout << "Error: listen error";
		exit(0);
	}

	FD_ZERO(&serverdata.master_fd);
	FD_SET(serverdata.socket_fd, &serverdata.master_fd);

	serverdata.max_sd = serverdata.socket_fd;
}

bool	Server::check_password(std::string password){
	password.erase(0, 5);
	size_t pos = password.find_first_of("\r\n");
	if (pos != std::string::npos)
		password.erase(pos);
	if (password == this->password)
		return true;
	return false;
}

void	Server::create_user(std::string msg, int sd){
	std::string username;
	std::string realname;
	std::string hostanme;
	std::string servername;

	msg.erase(0, 5);
	username = msg.substr(0, msg.find_first_of(" "));
	msg.erase(0, msg.find_first_of(" ") + 1);

	hostanme = msg.substr(0, msg.find_first_of(" "));
	msg.erase(0, msg.find_first_of(" ") + 1);

	servername = msg.substr(0, msg.find_first_of(" "));
	msg.erase(0, msg.find_first_of(" ") + 1);

	realname = msg;
	realname.erase(0, 1);
	
	size_t pos = realname.find_first_of("\r\n");
	if (pos != std::string::npos)
		realname.erase(pos);
		
	User *user = find_by_sd(sd);
    
    if (user == NULL) {
        std::cout << "Errore: utente non trovato!" << std::endl;
        return;
    }

    user->setAll(username, hostanme, servername, realname);
}

User* Server::find_by_sd(int sd){
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i]->sd == sd) {
            return users[i];
        }
    }
    return NULL;
}

Channel* Server::findChannelByName(std::string name) {
	for (size_t i = 0; i < allChannels.size(); i++) {
		if (allChannels[i].getChannelName() == name) {
			return &allChannels[i];
		}
	}
	return NULL;
}

User	*Server::find_by_nickname(std::string nickname){
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i]->getNickName() == nickname) {
			return users[i];
		}
	}
	return NULL;
}

void	Server::sendPrivmsg(std::string msg, User* sender)
{
	size_t pos = msg.find(" ");
	std::string channelName = msg.substr(0, pos);
	msg.erase(0, pos);
	pos = msg.find(":");
	msg.erase(0, pos + 1);
	std::cout << msg << std::endl;
	for (std::vector<Channel>::iterator it = allChannels.begin(); it != allChannels.end(); it++)
	{;
		if (it->getChannelName() == channelName)
		{
			std::vector<User> channelUsers = it->getUsers();
			for (size_t i = 0; i < channelUsers.size(); i++)
			{
				std::string fullMsg = ":" + sender->getNickName() + "!" + sender->getUserName() + "@localhost PRIVMSG " + channelName + " :" + msg + "\r\n";
				if (channelUsers[i].sd != sender->sd)
					send(channelUsers[i].sd, fullMsg.c_str(), fullMsg.length(), 0);
			}
		}
	}
	
}
