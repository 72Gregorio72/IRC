#include "Server.hpp"

void	Server::reply_to_user(int numErrno, std::string nickname, int sd){
	std::string msg = "";

	switch (numErrno) {
		case ERR_NICKNAMEINUSE:
			msg += ":127.0.0.1 433  *" + nickname + " :Nickname is already in use\r\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg = "Unknown command.";
			break;
		default:
			break;
	}
	send(sd, msg.c_str(), msg.length(), 0);
}