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

	int max_sd;
	fd_set master_fd;
	fd_set read_fd;
	
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

	FD_ZERO(&master_fd);
	FD_SET(socket_fd, &master_fd);

	max_sd = socket_fd;
	while(true){
		read_fd = master_fd;

		int activity = select(max_sd + 1, &read_fd, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			printf("Errore in select\n");
		}
		
		for(int sd = 0; sd <= max_sd; sd++){
			if (FD_ISSET(sd, &read_fd)){
				if (sd == socket_fd){
					socklen_t client_len = sizeof(client_sock);
					accept_fd = accept(socket_fd, (struct sockaddr *) &client_sock, &client_len);
					if (accept_fd < 0){
						std::cout << "Error: listen error";
						exit(0);
					}

					FD_SET(accept_fd, &master_fd);

					if (accept_fd > max_sd) {
						max_sd = accept_fd;
					}

					printf("New client connected to socket: %d\n", accept_fd);
				} else {
					bzero(msg, 256);
					return_value = recv(sd, msg, sizeof(msg) - 1, 0);
					if (return_value != -1){
						if (!return_value){
							printf("Client disconnected from socket: %d\n", sd);
							close(sd);
							FD_CLR(sd, &master_fd);
						} else {
							std::cout << "MSG: " << msg << std::endl;
							send(sd, msg, return_value, 0);
						}
					}
					else{
						perror("recv error");
						close(sd);
                    	FD_CLR(sd, &master_fd);
					}
				}
			}
		}
	}
	
	close(socket_fd);
	close(accept_fd);
	return 0; 
}