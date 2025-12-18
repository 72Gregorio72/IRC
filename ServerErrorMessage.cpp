#include "Server.hpp"

void	Server::replyErrToClient(int numErrno, std::string nickname, std::string channel, int sd, std::string arg){
	std::string msg = "";
	(void)arg;
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

		case ERR_NOTONCHANNEL:
			msg += ":localhost 442 " + nickname + " " + channel + " :You're not on that channel\r\n";
			break;

		case ERR_USERONCHANNEL:
			msg += ":localhost 443 " + nickname + " " + arg + " " + channel + " :Is already on channel\r\n";
			break;

		case ERR_NOSUCHNICK:
			msg = ":localhost 401 " + nickname + " " + channel + " :No such nick/channel\r\n";
            break;
		
		case ERR_CANNOTSENDTOCHAN:
			msg = ":localhost 404 " + nickname + " " + channel + " :Cannot send to channel\r\n";
			break;

		case ERR_USERNOTINCHANNEL:
            msg = ":localhost 441 " + nickname + " " + channel + " :User is not in that channel\r\n";
            break;

        case ERR_NOSUCHCHANNEL:
            msg = ":localhost 403 " + nickname + " " + channel + " :No Channel found\r\n";
            break;

        case ERR_CHANOPRIVSNEEDED:
            msg = ":localhost 482 " + nickname + " " + channel + " :You're not channel operator\r\n";
            break;

		case ERR_INVITEONLYCHAN:
			msg = ":localhost 473 " + channel + arg + "\r\n";
			break;

		case ERR_NEEDMOREPARAMS:
			msg = ":localhost 461 " + nickname + " " + arg + " :Not enough parameters\r\n"; 
			break;

		case ERR_BADCHANNELKEY:
			msg = ":localhost 475 " + nickname + " " + channel + " :Cannot join channel (+k)\r\n";
			break;

		case ERR_CHANNELISFULL:
			msg = ":localhost 471 " + nickname + " " + channel + " :Cannot join channel (+l)\r\n";
			break;

		case ERR_UNKNOWNMODE:
			msg = ":localhost 472 " + nickname + " " + arg + " :Is unknown mode char to me\r\n";
			break ;
	
		default:
			break;
	}
	std::cout << "msg to send: " << msg << std::endl;
	send(sd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
}