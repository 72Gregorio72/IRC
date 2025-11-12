#include "ft_irc.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void	open_server(data *data, char **av){
	data->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	data->port_number = std::atoi(av[1]);
	bzero((char *) &data->server_sock, sizeof(data->server_sock));
	data->server_sock.sin_port = htons(data->port_number);
	data->server_sock.sin_family = AF_INET;
	data->server_sock.sin_addr.s_addr = INADDR_ANY;
	if (bind(data->socket_fd, (struct sockaddr *) &data->server_sock, sizeof(data->server_sock)) < 0){
		std::cout << "Error: bind error";
		exit(0);
	}
	if (listen(data->socket_fd, 5) < 0){
		std::cout << "Error: listen error";
		exit(0);
	}

	FD_ZERO(&data->master_fd);
	FD_SET(data->socket_fd, &data->master_fd);

	data->max_sd = data->socket_fd;
}

void	server_loop(data *data){
	while(true){
		data->read_fd = data->master_fd;

		int activity = select(data->max_sd + 1, &data->read_fd, NULL, NULL, NULL);

		if ((activity < 0) && (errno != EINTR)) {
			printf("Errore in select\n");
		}
		
		for(int sd = 0; sd <= data->max_sd; sd++){
			if (FD_ISSET(sd, &data->read_fd)){
				if (sd == data->socket_fd){
					socklen_t client_len = sizeof(data->client_sock);
					data->accept_fd = accept(data->socket_fd, (struct sockaddr *) &data->client_sock, &client_len);
					if (data->accept_fd < 0){
						std::cout << "Error: listen error";
						exit(0);
					}

					FD_SET(data->accept_fd, &data->master_fd);

					if (data->accept_fd > data->max_sd) {
						data->max_sd = data->accept_fd;
					}

					printf("New client connected to socket: %d\n", data->accept_fd);
				} else {
					bzero(data->msg, 256);
					data->return_value = recv(sd, data->msg, sizeof(data->msg) - 1, 0);
					if (data->return_value != -1){
						if (!data->return_value){
							printf("Client disconnected from socket: %d\n", sd);
							close(sd);
							FD_CLR(sd, &data->master_fd);
						} else {
							std::cout << "MSG: " << data->msg << std::endl;
							parse_msg(data, data->msg);
							send(sd, data->msg, data->return_value, 0);
						}
					}
					else{
						perror("recv error");
						close(sd);
                    	FD_CLR(sd, &data->master_fd);
					}
				}
			}
		}
	}
}

int main(int ac, char *av[])
{
	data data;
	
	if (ac < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	open_server(&data, av);
	server_loop(&data);
	close(data.socket_fd);
	close(data.accept_fd);
	return 0; 
}