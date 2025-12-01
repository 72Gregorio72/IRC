#include "Server.hpp"

int	Server::parse_msg(int sd){
	std::string msg(serverdata.msg);

	if (find_by_sd(sd)->getNickName() == "") {
		if (msg.find("CAP LS 302") != std::string::npos)
			msg.erase(0, 12);

		if (msg.find("PASS ") != std::string::npos) {
			if (check_password(msg) && !find_by_sd(sd)->authenticated && find_by_sd(sd)->getNickName() == "") {
				find_by_sd(sd)->authenticated = true;
				return 1;
			} else {
				replyErrToClient(ERR_PASSWDMISMATCH, "", "", sd);
				return -72;
			}
		}

		if (size_t pos = msg.find("NICK ") != std::string::npos && find_by_sd(sd)->getNickName() == "" && find_by_sd(sd)->authenticated) {
			if (pos != std::string::npos)
				msg.erase(0, pos);
			msg.erase(0, 4);
			std::string nickname = msg.substr(0, msg.find_first_of("\n"));
			if (find_by_nickname(nickname) != NULL) {
				replyErrToClient(ERR_NICKNAMEINUSE, nickname, "", sd);
				return -72;
			}
			msg.erase(0, msg.find_first_of("\n"));
			find_by_sd(sd)->setNickName(nickname);
		}
	}
	if (!find_by_sd(sd)->authenticated)
		return -1;

	if (msg.find("USER ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
		create_user(msg, sd);
	}

	if (msg.find("QUIT ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
		remove_user(sd);
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
		if (pos < 2) // sender 
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
			replyErrToClient(ERR_NOTONCHANNEL, userToRemove->getNickName(), channelWanted->getChannelName(), sd);
			return (-72);
		}

		std::string msgToSend = ":" + userToRemove->getNickName() + "!" + userToRemove->getUserName() + "@localhost PART " + channelName + " :Leaving\r\n";

			// 5. Broadcast (Invio a tutti nel canale)
		std::vector<User> usersInChannel = channelWanted->getUsers();
		for (size_t i = 0; i < usersInChannel.size(); i++) {
			send(usersInChannel[i].sd, msgToSend.c_str(), msgToSend.length(), 0);
		}

		// 6. Rimozione e Pulizia
		channelWanted->removeUser(userToRemove->getNickName());
		
		if (channelWanted->getUsers().empty()) {
			deleteChannel(channelName);
			std::cout << "Channel " << channelName << " deleted." << std::endl;
		}
	}
	return 0;
}