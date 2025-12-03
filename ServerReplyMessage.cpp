#include "Server.hpp"

void    Server::replyServToClient(int numErrno, std::string nickname, int sd, std::string channelName, std::string addMsg)
{
    std::string msg = "";

	switch (numErrno) {
		case RPL_NAMREPLY:
			msg += ":localhost 353 " + nickname + " = " + channelName + " :" + addMsg + "\r\n";
			break;
		case RPL_ENDOFNAMES:
			msg += ":localhost 366 " + nickname + " " + channelName + " :" + addMsg + "\r\n";
			break;
		case RPL_NOTOPIC:
			msg += ":localhost 331 " + nickname + " " + channelName + " :No topic is set\r\n";
			break;
		case RPL_TOPIC:
			msg += ":localhost 332 " + nickname + " " + channelName + " :" + addMsg + "\r\n";
			break;
		default:
			break;
	}
	send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}