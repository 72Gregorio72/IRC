#include "Server.hpp"

Server::Server() : password("1234") {}

Server::~Server() {}

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

void	Server::add_user(User &newUser){
	users.insert(std::make_pair(newUser.getNickName(), newUser));
}

void	Server::remove_user(std::string nickname){
	users.erase(nickname);
}

void	Server::print_users(){
	std::cout << "Current users:" << std::endl;
	for (std::map<std::string, User>::iterator it = users.begin(); it != users.end(); ++it) {
		std::cout << "- " << it->first << std::endl;
	}
}

std::map <std::string, User> Server::getUsers(){
	return users;
}

void	Server::open_server(char **av){
	serverdata.socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
    if (setsockopt(serverdata.socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
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
	std::cout << "Received password: " << password << std::endl;
	std::cout << "Expected password: " << this->password << std::endl;
	return false;
}

void	Server::create_user(std::string msg){
	std::string nickname;
	std::string username;
	std::string realname;
	std::string hostanme;
	std::string servername;

	nickname = msg.substr(0, msg.find_first_of("\n"));
	msg.erase(0, msg.find_first_of("\n"));

	msg.erase(0, 6);

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

	User new_user = User(nickname, username, hostanme, servername, realname);
	add_user(new_user);
	std::cout << "User " << nickname << " created!" << std::endl;
	std::cout << "Username: " << username << std::endl;
	std::cout << "Hostname: " << hostanme << std::endl;
	std::cout << "Servername: " << servername << std::endl;
	std::cout << "Realname: " << realname << std::endl;
}

void	Server::parse_msg(){
	std::string msg(serverdata.msg);
	if (msg.find("CAP LS 302") != std::string::npos)
			msg.erase(0, 12);
	std::cout << "Parsing message: " << msg << std::endl;
	if (msg.find("PASS ") != std::string::npos) {
		if (check_password(msg)){
			std::cout << "Authentication correct!" << std::endl;
		} else {
			std::cout << "Authentication failed!" << std::endl;
		}
	}
	if (size_t pos = msg.find("NICK ") != std::string::npos) {
		if (pos != std::string::npos)
			msg.erase(0, pos);
		msg.erase(0, 4);
		create_user(msg);
	}

	if (msg.find("printusers ") != std::string::npos) {
		print_users();
	}
}

void	Server::server_loop(){
	while(true){
		serverdata.read_fd = serverdata.master_fd;

		int activity = select(serverdata.max_sd + 1, &serverdata.read_fd, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			printf("Errore in select\n");
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

					printf("New client connected to socket: %d\n", serverdata.accept_fd);
				} else {
					bzero(serverdata.msg, 256);
					serverdata.return_value = recv(sd, serverdata.msg, sizeof(serverdata.msg) - 1, 0);
					if (serverdata.return_value != -1){
						if (!serverdata.return_value){
							printf("Client disconnected from socket: %d\n", sd);
							close(sd);
							FD_CLR(sd, &serverdata.master_fd);
						} else {
							std::cout << "MSG: " << serverdata.msg << std::endl;
							parse_msg();
							send(sd, serverdata.msg, serverdata.return_value, 0);
						}
					}
					else{
						perror("recv error");
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