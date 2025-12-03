#include "Server.hpp"

// void Server::assignPassword(std::string msg)
// {

// }

int Server::kick(std::string msg, int sd){
	size_t pos = msg.find("KICK ");
	if (pos != std::string::npos) msg.erase(0, pos);
	msg.erase(0, 5);
	std::string channelName = msg.substr(0, msg.find(" "));
	msg.erase(0, msg.find(" ") + 1);
	std::string userToKick;
	size_t endPos = msg.find_first_of(" \r\n");
	if (endPos != std::string::npos)
		userToKick = msg.substr(0, endPos);
	else
		userToKick = msg;
	std::string reason = "";
	if (msg.find(" :") != std::string::npos){
		reason = msg.substr(msg.find(" :") + 2, msg.find("\r\n") - (msg.find(" :") + 2));
	}
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
		std::string partMsg = ":" + userToKick + "!" + userToKick + "@localhost PART " + channelName + " :" + reason + "\r\n";
		for (size_t j = 0; j < users.size(); j++) {
			int fd = users[j].sd;
			send(fd, partMsg.c_str(), partMsg.length(), MSG_NOSIGNAL);
		}
	}
	channel->removeUser(userToKick);
	return 0;
}

int Server::part(std::string msg, int sd){
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

	if (!channelWanted->userInChannel(userToRemove->getNickName()) && !channelWanted->getUsers().empty()) {
		replyErrToClient(ERR_NOTONCHANNEL, userToRemove->getNickName(), channelWanted->getChannelName(), sd, "");
		return (-72);
	}
	
	channelWanted->removeUser(userToRemove->getNickName());
	
	if (channelWanted->getUsers().empty()) {
		deleteChannel(channelName);
		std::cout << "Channel " << channelName << " deleted." << std::endl;
	}
	return (0);
}

int Server::parse_entry(std::string msg, int sd){
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
	return 0;
}

int Server::parse_join(std::string msg) {
	std::string temp = msg;
	temp.find(",");
	temp.erase(0, temp.find(",") + 1);
	if (temp[0] == ' ' || temp[0] == '\r' || temp[0] == '\n')
		return -1;
	temp = msg;
	temp.erase(0, temp.find(" ") + 1);
	if (temp.empty())
		return -1;
	if (temp[0] == ',' || temp[0] == '\r' || temp[0] == '\n')
		return -1;
	return 0;
}

int Server::parse_msg(int sd) {
    if (sd < 0)
        return -1;
    
    std::string msg(serverdata.msg);

    if (parse_entry(msg, sd) == -72)
        return -72;

    if (!find_by_sd(sd)->authenticated)
        return -1;

    if (msg.find("USER ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
        create_user(msg, sd);
        return 0;
    }

    if (msg.find("QUIT ") != std::string::npos) {
        for (size_t i = 0; i < allChannels.size(); i++)
			if (allChannels[i].userInChannel(find_by_sd(sd)->getNickName()))
            	allChannels[i].removeUser(find_by_sd(sd)->getNickName());
		remove_user(sd);
        return 0;
    }

    if (msg.find("JOIN ") != std::string::npos) {
		std::cout << msg << std::endl;
        size_t pos = msg.find("JOIN ");
        if (pos != std::string::npos) msg.erase(0, pos);
        msg.erase(0, 5);
        pos = msg.find_first_of("\r\n");
		if (parse_join(msg) == -1) {
			replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), "", sd, "");
			return -1;
		}
        if (pos != std::string::npos) msg = msg.substr(0, pos);
		std::string channels = msg.substr(0, msg.find(" "));
		if (msg.find(" ") != std::string::npos)
			msg.erase(0, msg.find(" ") + 1);
		else
			msg.erase(0, msg.find("\r\n"));
		std::string passwords = msg.substr(0, msg.find(" "));
		if (passwords.empty())
			passwords = "";
		std::cout << "Channels to join: |" << channels << "|" << std::endl;
		std::cout << "Passwords provided: " << passwords << std::endl;
		if (channels.find(" ") != std::string::npos || channels.find(7) != std::string::npos) {
			if (passwords.empty() && (passwords.find(" ") != std::string::npos || passwords.find(7) != std::string::npos)) {
				replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channels, sd, "");
				return -1;
			}
		}
		do {
			pos = channels.find_first_of(",");
			std::string channelName;;
			if (pos != std::string::npos)
			{
				channelName = channels.substr(0, pos);
				channels.erase(0, pos + 1);
			}
			else
				channelName = channels;
			if (channelName.find("#") != 0 && channelName.find("&") != 0) {
				channelName = "#" + channelName;
			}
			// if (channelName.find_first_of(",") != std::string::npos || channelName.find(" ") != std::string::npos || channelName.size() <= 1)
			// {
			// 	replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
			// 	continue ;
			// }
			// assignPassword(msg, channelName);
			Channel channel(channelName, this);
			if (findChannelByName(channel.getChannelName()) != NULL) {
				Channel* existingChannel = findChannelByName(channel.getChannelName());
				std::vector<User> users = existingChannel->getUsers();
				for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it)
				{
					if (it->sd != sd)
					{
						existingChannel->addUser(find_by_sd(sd));
						continue ;
					}
				}
			}
			if (findChannelByName(channel.getChannelName()) == NULL)
			{
				allChannels.push_back(channel);
				allChannels.back().addUser(find_by_sd(sd));
			}
		}
		while (pos != std::string::npos);
    }

    else if (msg.find("PRIVMSG ") != std::string::npos) {
        size_t pos = msg.find("PRIVMSG ");
        if (pos != std::string::npos) 
        {
            msg.erase(0, pos + 8);
            pos = msg.find_first_of("\r\n");
            if (pos != std::string::npos)
                msg = msg.substr(0, pos);
            sendPrivmsg(msg, find_by_sd(sd));
        }
        return 0;
    }

    else if (msg.find("PART ") != std::string::npos) {
        return (part(msg, sd));
    }

    else if (msg.find("KICK ") != std::string::npos) {
        return (kick(msg, sd));
    }
    if (msg.find("printusers") != std::string::npos) {
        print_users();
        return 0;
    }

    return 0;
}