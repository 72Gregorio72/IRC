#ifndef FT_IRC_H
#define FT_IRC_H

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
#include <sstream>
#include <stdlib.h>
#include <cmath>

#include "macros.h"

#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

#include "Channel.hpp"
#include "User.hpp"
#include "Server.hpp"

extern volatile sig_atomic_t g_sig_received;

struct flagArg{
	int flag;
	std::string arg;
} typedef flagArg;

struct flags{
	flagArg i;
	flagArg	t;
	flagArg	k;
	flagArg	o;
	flagArg	l;
	int		setting;
} typedef flags;

#endif
