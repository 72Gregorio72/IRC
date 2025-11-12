/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gpicchio <gpicchio@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:56:57 by vcastald          #+#    #+#             */
/*   Updated: 2025/11/12 11:44:02 by gpicchio         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <iostream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <climits>

#include <map>
#include <vector>
#include <algorithm>

#include "Channel.hpp"
#include "User.hpp"

struct data{
	int socket_fd, port_number, accept_fd;
	struct sockaddr_in server_sock, client_sock;
	char msg[256];
	ssize_t return_value;
	int max_sd;
	fd_set master_fd;
	fd_set read_fd;
};


void	parse_msg(data *data, std::string msg);
