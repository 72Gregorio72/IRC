#include "Server.hpp"

Server::Server() : password("1234") {}

Server::~Server() {
    for (size_t i = 0; i < users.size(); i++) {
        delete users[i];
    }
    users.clear();
}

Server::Server(std::string pass) : password(pass) {}

Server::Server(const Server &other) : password(other.password) {}

Server& Server::operator=(const Server &other){
	if (this == &other)
		return *this;
	password = other.password;
	return *this;
}

std::string Server::getPassword(){
	return password;
}

void	Server::add_user(User *newUser){
	users.push_back(newUser);
}

void Server::remove_user(int sd){
    for (std::vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if ((*it)->sd == sd) {
            std::cout << "Removing user with socket: " << sd << std::endl;
            FD_CLR((*it)->sd, &serverdata.master_fd);
            close((*it)->sd); 
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
	// std::cout << "Received password: " << password << std::endl;
	// std::cout << "Expected password: " << this->password << std::endl;
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
	realname.erase(0, 1); // real name removing :
	size_t pos = realname.find_first_of("\r\n");
	if (pos != std::string::npos)
		realname.erase(pos);
		
	User *user = find_by_sd(sd);
    
    if (user == NULL) {
        std::cout << "Errore: utente non trovato!" << std::endl;
        return;
    }

    // Usa la freccia -> per modificare l'oggetto REALE
    user->setAll(username, hostanme, servername, realname);
}

int	Server::parse_msg(int sd){
	std::string msg(serverdata.msg);

	if (find_by_sd(sd)->getNickName() == "") {
		std::cout << "User not found for socket: " << sd << std::endl;
		if (msg.find("CAP LS 302") != std::string::npos)
			msg.erase(0, 12);

		if (msg.find("PASS ") != std::string::npos) {
			if (check_password(msg)){
				std::cout << "Authentication correct!" << std::endl;
				find_by_sd(sd)->authenticated = true;
			} else {
				std::cout << "Authentication failed!" << std::endl;
				return -72;
			}
		}

		if (size_t pos = msg.find("NICK ") != std::string::npos && find_by_sd(sd)->getNickName() == "" && find_by_sd(sd)->authenticated) {
			if (pos != std::string::npos)
				msg.erase(0, pos);
			msg.erase(0, 4);
			std::string nickname = msg.substr(0, msg.find_first_of("\n"));
			msg.erase(0, msg.find_first_of("\n"));
			find_by_sd(sd)->setNickName(nickname);
			std::cout << "Nickname set to: " << nickname << std::endl;
		}
	}
	if (!find_by_sd(sd)->authenticated)
		return -1;

	if (msg.find("USER ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
		create_user(msg, sd);
	}

	if (msg.find("printusers") != std::string::npos) {
		print_users();
	}
	return 0;
}

User* Server::find_by_sd(int sd){
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i]->sd == sd) {
            return users[i];
        }
    }
    return NULL;
}

void	Server::server_loop(){
	while(true){
		serverdata.read_fd = serverdata.master_fd;

		int activity = select(serverdata.max_sd + 1, &serverdata.read_fd, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			std::cout << "Error: select error" << std::endl;
			exit(0);
		}
		
		for(int sd = 0; sd <= serverdata.max_sd; sd++){
			if (FD_ISSET(sd, &serverdata.read_fd)){

				if (sd == serverdata.socket_fd){
					socklen_t client_len = sizeof(serverdata.client_sock);
					serverdata.accept_fd = accept(serverdata.socket_fd, (struct sockaddr *) &serverdata.client_sock, &client_len);
					if (serverdata.accept_fd < 0){
						std::cout << "Error: listen error";
						exit(0);
					}

					FD_SET(serverdata.accept_fd, &serverdata.master_fd);

					if (serverdata.accept_fd > serverdata.max_sd) {
						serverdata.max_sd = serverdata.accept_fd;
					}
					User *new_user = new User(serverdata.accept_fd);
					new_user->buffer = "";
					add_user(new_user);
				} else {
					char buffer[256];
					ssize_t bytes_received = recv(sd, buffer, sizeof(buffer) - 1, 0);
					if (bytes_received > 0) {
						buffer[bytes_received] = '\0';
						User* user = find_by_sd(sd);
						if (user == NULL) {
							std::cout << "User not found for socket: " << sd << std::endl;
							continue;
						}
						user->buffer += std::string(buffer);
						size_t pos;
						while ((pos = user->buffer.find("\r\n")) != std::string::npos)
						{
							std::string complete_msg = user->buffer.substr(0, pos + 2);
							// std::cout << "Complete message from socket " << sd << ": " << complete_msg << std::endl;
							serverdata.msg[0] = '\0';
							strncpy(serverdata.msg, complete_msg.c_str(), sizeof(serverdata.msg) - 1);
							if (parse_msg(sd) == -72) {
								remove_user(sd);
								break;
							}
							user->buffer.erase(0, pos + 2);
						}
					} else if (bytes_received == 0) {
						std::cout << "Socket " << sd << " disconnected." << std::endl;
						close(sd);
						FD_CLR(sd, &serverdata.master_fd);
						User *user = find_by_sd(sd);
						if (user != NULL) {
							remove_user(user->sd);
						}
					} else {
						std::cout << "Error: recv error" << std::endl;
						close(sd);
					   	FD_CLR(sd, &serverdata.master_fd);
					}
				}
			}
		}
	}
}

void	Server::close_all()
{
	close(serverdata.accept_fd);
	close(serverdata.socket_fd);
}