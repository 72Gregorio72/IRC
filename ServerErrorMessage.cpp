#include "Server.hpp"

void	Server::replyErrToClient(int numErrno, std::string nickname, int sd){
	std::string msg = "";

	switch (numErrno) {
		case ERR_NICKNAMEINUSE:
			msg += ":localhost 433  *" + nickname + " :Nickname is already in use\r\n";
			break;
		case ERR_UNKNOWNCOMMAND:
			msg = "Unknown command.";
			break;
		case ERR_PASSWDMISMATCH:
			msg += ":localhost 464 * :Password incorrect\r\n";
			break;
		default:
			break;
	}
	send(sd, msg.c_str(), msg.length(), 0);
}