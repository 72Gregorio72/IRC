/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcastald <vcastald@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 08:56:57 by vcastald          #+#    #+#             */
/*   Updated: 2025/11/05 08:58:19 by vcastald         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Librerie standard C++ (versione <c...>)
#include <cstdlib>      // malloc, free, exit, atoi
#include <cstdio>       // printf, perror, snprintf
#include <cstring>      // strerror, memcpy, memset, etc.
#include <cerrno>       // errno
#include <csignal>      // signal, SIGINT, SIGTERM, ecc.
#include <iostream>

// Librerie di sistema POSIX (non esistono equivalenti in <c...>)
#include <unistd.h>     // fork, execve, pipe, dup, dup2, read, write, close, chdir, access
#include <fcntl.h>      // fcntl, open
#include <sys/types.h>  // pid_t, size_t, ssize_t
#include <sys/stat.h>   // stat, fstat
#include <dirent.h>     // opendir, readdir, closedir
#include <sys/wait.h>   // waitpid
#include <sys/time.h>   // struct timeval (per select)
#include <sys/select.h> // select, FD_SET, FD_ISSET, ecc.
#include <poll.h>       // poll
#include <sys/epoll.h>  // epoll_create, epoll_ctl, epoll_wait (solo Linux)
#include <sys/socket.h> // socket, bind, connect, listen, accept, send, recv, setsockopt, getsockname
#include <netinet/in.h> // sockaddr_in, htons, htonl, ntohs, ntohl
#include <arpa/inet.h>  // inet_pton, inet_ntop
#include <netdb.h>      // getaddrinfo, freeaddrinfo, gai_strerror, getprotobyname
#include <signal.h>     // kill (opzionale, anche in <csignal>)
