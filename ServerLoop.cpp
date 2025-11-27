#include "Server.hpp"

void Server::server_loop() {
	while (true) {
		serverdata.read_fd = serverdata.master_fd;

		int activity = select(serverdata.max_sd + 1, &serverdata.read_fd, NULL, NULL, NULL);
		if ((activity < 0) && (errno != EINTR)) {
			std::cout << "Error: select error" << std::endl;
			exit(0);
		}

		for (int sd = 0; sd <= serverdata.max_sd; ++sd) {
			if (FD_ISSET(sd, &serverdata.read_fd)) {
				if (sd == serverdata.socket_fd) {
					handle_new_connection();
				} else {
					handle_client_read(sd);
				}
			}
		}
	}
}

void Server::handle_new_connection() {
	socklen_t client_len = sizeof(serverdata.client_sock);
	serverdata.accept_fd = accept(serverdata.socket_fd, (struct sockaddr *)&serverdata.client_sock, &client_len);
	if (serverdata.accept_fd < 0) {
		std::cout << "Error: accept error" << std::endl;
		return;
	}

	FD_SET(serverdata.accept_fd, &serverdata.master_fd);
	if (serverdata.accept_fd > serverdata.max_sd) {
		serverdata.max_sd = serverdata.accept_fd;
	}

	User *new_user = new User(serverdata.accept_fd);
	new_user->buffer = "";
	add_user(new_user);
}

void Server::handle_client_read(int sd) {
	char buffer[256];
	ssize_t bytes_received = recv(sd, buffer, sizeof(buffer) - 1, 0);

	if (bytes_received > 0) {
		buffer[bytes_received] = '\0';
		User *user = find_by_sd(sd);
		if (user == NULL) {
			std::cout << "User not found for socket: " << sd << std::endl;
			FD_CLR(sd, &serverdata.master_fd);
			close(sd);
			return;
		}
		user->buffer += std::string(buffer);

		int result = process_user_buffer(user, sd);
		(void)result;
	} else if (bytes_received == 0) {
		FD_CLR(sd, &serverdata.master_fd);
		User *user = find_by_sd(sd);
		if (user != NULL) {
			remove_user(user->sd);
		}
	} else {
		std::cout << "Error: recv error" << std::endl;
		FD_CLR(sd, &serverdata.master_fd);
	}
}

int Server::process_user_buffer(User *user, int sd) {
	size_t pos;
	while ((pos = user->buffer.find("\r\n")) != std::string::npos) {
		std::string complete_msg = user->buffer.substr(0, pos + 2);
		std::cout << "Complete message from socket " << sd << ": " << complete_msg << std::endl;

		serverdata.msg[0] = '\0';
		strncpy(serverdata.msg, complete_msg.c_str(), sizeof(serverdata.msg) - 1);

		int number = parse_msg(sd);
		if (number == -72) {
			remove_user(sd);
			std::cout << "Authentication Failed! " << sd << std::endl;
			return -72;
		} else if (number == 1) {
			std::cout << "Authentication Correct! " << sd << std::endl;
		}
		user->buffer.erase(0, pos + 2);
	}
	return 0;
}

void	Server::close_all()
{
	close(serverdata.accept_fd);
	close(serverdata.socket_fd);
}