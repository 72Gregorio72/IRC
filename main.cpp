#include "ft_irc.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int ac, char *av[])
{
	int socket_fd, port_number, accept_fd;
	struct sockaddr_in server_sock, client_sock;
	char msg[256];
	ssize_t return_value;
	
	if (ac < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	port_number = std::atoi(av[1]);
	bzero((char *) &server_sock, sizeof(server_sock));
	server_sock.sin_port = htons(port_number);
	server_sock.sin_family = AF_INET;
	server_sock.sin_addr.s_addr = INADDR_ANY;
	if (bind(socket_fd, (struct sockaddr *) &server_sock, sizeof(server_sock)) < 0){
		std::cout << "Error: bind error";
		exit(0);
	}
	if (listen(socket_fd, 5) < 0){
		std::cout << "Error: listen error";
		exit(0);
	}
	socklen_t client_len = sizeof(client_sock);
	accept_fd = accept(socket_fd, (struct sockaddr *) &client_sock, &client_len);
	if (accept_fd < 0){
		std::cout << "Error: listen error";
		exit(0);
	}

	while(true){
		bzero(msg, 256);
		return_value = recv(accept_fd, msg, sizeof(msg) - 1, 0);
		if (return_value != -1)
			std::cout << "Message: " << msg << std::endl;
		else{
			std::cout << "Error: Ti piace il pisello" << std::endl;
		}
	}
	
	close(socket_fd);
	close(accept_fd);
	return 0; 
}