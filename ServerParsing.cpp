#include "Server.hpp"

int	Server::parse_msg(int sd){
	if (sd < 0)
		return -1;
	std::string msg(serverdata.msg);

	if (find_by_sd(sd)->getNickName() == "") {
		if (msg.find("CAP LS 302") != std::string::npos) {
			msg.erase(0, 12);
			return 0;
		}

		if (msg.find("PASS ") != std::string::npos) {
			if (check_password(msg) && !find_by_sd(sd)->authenticated && find_by_sd(sd)->getNickName() == "") {
				find_by_sd(sd)->authenticated = true;
				return 1;
			} else {
				replyErrToClient(ERR_PASSWDMISMATCH, "", "", sd, "");
				return -72;
			}
		}
		if (size_t pos = msg.find("NICK ") != std::string::npos && find_by_sd(sd)->getNickName() == "" && find_by_sd(sd)->authenticated) {
			if (pos != std::string::npos)
				msg.erase(0, pos);
			msg.erase(0, 4);
			std::string nickname = msg.substr(0, msg.find_first_of("\r\n"));
			if (find_by_nickname(nickname) != NULL) {
				replyErrToClient(ERR_NICKNAMEINUSE, nickname, "", sd, "");
				return -72;
			}
			msg.erase(0, msg.find_first_of("\r\n"));
			find_by_sd(sd)->setNickName(nickname);
			return 0;
		}
	}

	if (!find_by_sd(sd)->authenticated)
		return -1;

	if (msg.find("USER ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
		create_user(msg, sd);
	}

	if (msg.find("QUIT ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
		remove_user(sd);
		for (size_t i = 0; i < allChannels.size(); i++)
			allChannels[i].removeUser(find_by_sd(sd)->getNickName());
	}

	if (msg.find("printusers") != std::string::npos) {
		print_users();
	}

	if (msg.find("JOIN ") != std::string::npos) {
		size_t pos = msg.find("JOIN ");
		if (pos != std::string::npos) msg.erase(0, pos);
		msg.erase(0, 5);
		pos = msg.find_first_of("\r\n");
		if (pos != std::string::npos) msg = msg.substr(0, pos);

		if (msg.find("#") != 0 && msg.find("&") != 0) {
			msg = "#" + msg;
		}
		Channel channel(msg, this);
		if (findChannelByName(channel.getChannelName()) != NULL) {
			Channel* existingChannel = findChannelByName(channel.getChannelName());
			existingChannel->addUser(find_by_sd(sd));
			return 0;
		}
		allChannels.push_back(channel);
		allChannels.back().addUser(find_by_sd(sd));
	}


	if (msg.find("PRIVMSG ") != std::string::npos) {
		size_t pos = msg.find("PRIVMSG ");
		if (pos < 2)
		{
			msg.erase(0, 8);
			pos = msg.find_first_of("\r\n");
			if (pos != std::string::npos)
				msg = msg.substr(0, pos);
			sendPrivmsg(msg, find_by_sd(sd));
		}
	}

	if (msg.find("PART ") != std::string::npos) {
		size_t pos = msg.find("PART ");
		if (pos != std::string::npos) msg.erase(0, pos);
		msg.erase(0, 5);
		pos = msg.find_first_of("\r\n");
		if (pos != std::string::npos) msg = msg.substr(0, pos);
		User *userToRemove = find_by_sd(sd);
		std::string channelName = msg.substr(0, msg.find_first_of(' '));

		Channel *channelWanted = findChannelByName(channelName);
		if (!channelWanted)
		{
			std::cout << channelName << std::endl;
			return (-100);
		}
		if (!channelWanted->userInChannel(userToRemove->getNickName())) {
			replyErrToClient(ERR_NOTONCHANNEL, userToRemove->getNickName(), channelWanted->getChannelName(), sd, "");
			return (-72);
		}

		std::string msgToSend = ":" + userToRemove->getNickName() + "!" + userToRemove->getUserName() + "@localhost PART " + channelName + " :Leaving\r\n";
		std::vector<User> usersInChannel = channelWanted->getUsers();
		for (size_t i = 0; i < usersInChannel.size(); i++) {
			send(usersInChannel[i].sd, msgToSend.c_str(), msgToSend.length(), 0);
		}
		channelWanted->removeUser(userToRemove->getNickName());
		
		if (channelWanted->getUsers().empty()) {
			deleteChannel(channelName);
			std::cout << "Channel " << channelName << " deleted." << std::endl;
		}
	}

	if (msg.find("KICK ") != std::string::npos){
		size_t pos = msg.find("KICK ");
		if (pos != std::string::npos) msg.erase(0, pos);
		msg.erase(0, 5);
		std::string channelName = msg.substr(0, msg.find(" "));
		msg.erase(0, msg.find(" ") + 1);
		std::string userToKick = msg.substr(0, msg.find("\r\n"));
		Channel* channel = findChannelByName(channelName);
		if (channel == NULL){
			replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
			return -1;
		}
		if (channel->findUserByNickname(find_by_sd(sd)->getNickName())->_isOp() == false){
			replyErrToClient(ERR_CHANOPRIVSNEEDED, find_by_sd(sd)->getNickName(), channelName, sd, "");
			return -1;
		}
		if (channel->findUserByNickname(find_by_sd(sd)->getNickName()) == NULL){
			replyErrToClient(ERR_NOTONCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
			return -1;
		}
		if (channel->userInChannel(userToKick) == false){
			replyErrToClient(ERR_USERNOTINCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, channelName);
			return -1;
		} else {
			std::vector<User> users = channel->getUsers();
			std::string partMsg = ":" + userToKick + "!" + userToKick + "@127.0.0.1 PART " + channelName + "\r\n";
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, partMsg.c_str(), partMsg.length(), 0);
			}
		}
		channel->removeUser(userToKick);
	}
	return 0;
}