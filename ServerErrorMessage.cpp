#include "Server.hpp"

void	Server::replyErrToClient(int numErrno, std::string nickname, std::string channel,int sd){
void	Server::reply_to_user(int numErrno, std::string nickname, int sd, std::string arg){
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
		case ERR_NOTONCHANNEL:
			msg += ":localhost 442 " + nickname + channel + " :You're not on that channel\r\n";
			break;
			break;

		case ERR_NOSUCHNICK:
			msg = ":127.0.0.1 401 " + nickname + " " + arg + " :No such nick/channel\r\n";
            break;
		
		case ERR_USERNOTINCHANNEL:
            msg = ":127.0.0.1 441 " + nickname + " " + arg + " :They aren't on that channel\r\n";
            break;

        case ERR_NOSUCHCHANNEL:
            msg = ":127.0.0.1 403 " + nickname + " " + arg + " :No such channel\r\n";
            break;

        case ERR_NOTONCHANNEL:
             msg = ":127.0.0.1 442 " + nickname + " " + arg + " :You're not on that channel\r\n";
             break;

        case ERR_CHANOPRIVSNEEDED:
            msg = ":127.0.0.1 482 " + nickname + " " + arg + " :You're not channel operator\r\n";
            break;
		default:
			break;
	}
	std::cout << "msg to send: " << msg << std::endl;
	send(sd, msg.c_str(), msg.length(), 0);
}