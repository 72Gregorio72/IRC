#include "Server.hpp"
#include "BalatroBot/includes/Balatro.hpp"

bool Server::checkUserPassword(Channel *channel, std::string channelsCopy, std::string passwords)
{
	std::size_t chanPos;
	std::size_t passPos;

	while (!channelsCopy.empty()) {
		std::string chanName;
		chanPos = channelsCopy.find(",");
		if (chanPos != std::string::npos)
		{
			chanName = channelsCopy.substr(0, chanPos);
			channelsCopy.erase(0, chanPos + 1);
		}
		else
		{
			chanName = channelsCopy;
			channelsCopy.clear();
		}
		std::string pass;
		if (!passwords.empty()) 
		{
			passPos = passwords.find(",");
			if (passPos != std::string::npos)
			{
				pass = passwords.substr(0, passPos);
				passwords.erase(0, passPos + 1);
			}
			else
			{
				pass = passwords;
				passwords.clear();
			}
		}
		if (chanName[0] != '#' && chanName[0] != '&')
			chanName = "#" + chanName;
		//std:: cout << "Checking channel: " << chanName << " with password: " << pass << " against channel: " << channel->getChannelName() << " with password: " << channel->getPassword() << std::endl;
		if (chanName == channel->getChannelName())
		{
			if (channel->getPassword() != pass)
				return false;
			else
				return true;
		}
		chanName.clear();
		pass.clear();
	};
	return true;
}

bool Server::assignPasswordToChannel(std::string channels, std::string passwords, Channel *channel)
{
	std::size_t chanPos;
	std::size_t passPos;
	int countChannel = 0;
	int countPassword = 0;

	for (std::size_t i = 0; i < channels.length(); i++) {
		if (channels[i] == ',')
			countChannel++;
	}
	countChannel++;
	for (std::size_t i = 0; i < passwords.length(); i++) {
		if (passwords[i] == ',')
			countPassword++;
	}
	countPassword++;
	if (countPassword > countChannel)
		return false;
	while (!channels.empty()) {
		std::string chanName;
		chanPos = channels.find(",");
		if (chanPos != std::string::npos)
		{
			chanName = channels.substr(0, chanPos);
			channels.erase(0, chanPos + 1);
		}
		else
		{
			chanName = channels;
			channels.clear();
		}
		std::string pass;
		if (!passwords.empty()) 
        {
            passPos = passwords.find(",");
            if (passPos != std::string::npos)
            {
                pass = passwords.substr(0, passPos);
                passwords.erase(0, passPos + 1);
            }
            else
            {
                pass = passwords;
                passwords.clear();
            }
        }
		if (chanName[0] != '#' && chanName[0] != '&')
			chanName = "#" + chanName;
		if (chanName == channel->getChannelName())
		{
			channel->setPassword(pass);
			//std::cout << "Assigning password: " << pass << " to channel: " << chanName << std::endl;
			return true;
		}
		chanName.clear();
		pass.clear();
	};
	return true;
}

bool Server::alreadyInChannel(std::string nickname, std::string channelName) {
	Channel channel = *findChannelByName(channelName);
	if (channel.userInChannel(nickname))
		return true;
	return false;
}

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

    User* userSender = channel->findUserByNickname(find_by_sd(sd)->getNickName());

    if (userSender == NULL){
        replyErrToClient(ERR_NOTONCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
        return -1;
    }

    if (userSender->_isOp() == false){
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
	}
	std::string kickMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost KICK " + channelName + " " + userToKick;
	if (!reason.empty()){
		kickMsg += " :" + reason + "\r\n";
	} else {
		kickMsg += " :No reason specified\r\n";
	}
	send(find_by_sd(sd)->sd, kickMsg.c_str(), kickMsg.length(), MSG_NOSIGNAL);
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
		replyErrToClient(ERR_NOSUCHCHANNEL, userToRemove->getNickName(), channelName, sd, "");
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
			nickname.erase(nickname.find_last_not_of(" \r\n") + 1);
			find_by_sd(sd)->setNickName(nickname);
			find_by_sd(sd)->setHasNick(true);
			return 0;
		}
	}
	if (!find_by_sd(sd)->authenticated) {
		replyErrToClient(ERR_PASSWDMISMATCH, "", "", sd, "");
		return -72;
	}
	return 0;
}

int Server::topic(std::string msg, int sd) {
    User *userSender = find_by_sd(sd);

    size_t pos = msg.find("TOPIC");
    if (pos != std::string::npos) 
        msg = msg.substr(pos + 5); 
    pos = msg.find_last_not_of("\r\n");
    if (pos != std::string::npos) 
        msg = msg.substr(0, pos + 1);

    size_t chanStart = msg.find_first_not_of(" \t");
    if (chanStart == std::string::npos) {
        replyErrToClient(ERR_NEEDMOREPARAMS, userSender->getNickName(), "TOPIC", sd, "");
        return (-100);
    }
    size_t chanEnd = msg.find_first_of(" \t", chanStart);

    std::string channelName;
    std::string topicStr;
    bool settingTopic = false;

    if (chanEnd == std::string::npos) {
        channelName = msg.substr(chanStart);
        settingTopic = false;
    } 
    else {
        channelName = msg.substr(chanStart, chanEnd - chanStart);
        std::string leftover = msg.substr(chanEnd);

        size_t topicStart = leftover.find_first_not_of(" \t");
        
        if (topicStart != std::string::npos) {
            topicStr = leftover.substr(topicStart);
            if (topicStr[0] == ':') {
                topicStr = topicStr.substr(1);
            }
            settingTopic = true;
        }
    }

    Channel *wantedChannel = findChannelByName(channelName);
	if (!wantedChannel) {
        replyErrToClient(ERR_NOSUCHCHANNEL, userSender->getNickName(), channelName, sd, "");
        return (-100);
    }

	std::string nickSender = userSender->getNickName();
    userSender = wantedChannel->findUserByNickname(userSender->getNickName());
	if (!userSender) {
		replyErrToClient(ERR_NOTONCHANNEL, nickSender, wantedChannel->getChannelName(), sd, "");
		return (-100);
	}
    if (!settingTopic) {
        std::string topicWanted = wantedChannel->getTopic();
        if (topicWanted == "")
            replyServToClient(RPL_NOTOPIC, userSender->getNickName(), sd, wantedChannel->getChannelName(), "");
        else
            replyServToClient(RPL_TOPIC, userSender->getNickName(), sd, wantedChannel->getChannelName(), topicWanted);
    }

    else {

        if (wantedChannel->getTopicChangeOnlyOp() && userSender->_isOp() == false) {
            replyErrToClient(ERR_CHANOPRIVSNEEDED, userSender->getNickName(), wantedChannel->getChannelName(), sd, "");
            return (-100);
        }

        wantedChannel->setTopic(topicStr);
        std::string notifMsg = ":" + userSender->getNickName() + "!" + userSender->getUserName() + "@localhost TOPIC " + channelName + " :" + topicStr + "\r\n";
        
        std::vector <User> usersInChannel = wantedChannel->getUsers();
        for (size_t i = 0; i < usersInChannel.size(); i++) 
            send(usersInChannel[i].sd, notifMsg.c_str(), notifMsg.length(), MSG_NOSIGNAL);
    }

    return (0);
}

int Server::parse_join(std::string msg) {
	std::string temp = msg;
	temp.find(",");
	temp.erase(0, temp.find(",") + 1);
	if (temp[0] == ' ' || temp[0] == ',' || temp[0] == '\r' || temp[0] == '\n')
		return -1;
	temp = msg;
	temp.erase(0, temp.find(" ") + 1);
	if (temp.empty())
		return -1;
	if (temp[0] == ',' || temp[0] == '\r' || temp[0] == '\n')
		return -1;
	return 0;
}

int	Server::invite(std::string msg, int sd)
{
    User *userSender = find_by_sd(sd);
	if (!userSender)
        return (-1); 
	if (msg.substr(0, 7) == "INVITE ") 
        msg = msg.substr(7); 

    size_t pos_end = msg.find_last_not_of("\r\n");
    if (pos_end != std::string::npos) 
        msg = msg.substr(0, pos_end + 1);
    
    size_t spacePos = msg.find(" ");
	if (spacePos == std::string::npos)
	{
		replyErrToClient(ERR_NEEDMOREPARAMS, userSender->getNickName(), "INVITE", sd, "");
        return (-1);
	}
	std::string nickInvited = msg.substr(0, spacePos);
	std::string channelName = msg.substr(spacePos + 1);

	if (channelName.empty())
    {
        replyErrToClient(ERR_NEEDMOREPARAMS, userSender->getNickName(), "INVITE", sd, "");
        return (-1);
    }

	if (channelName.find("#") != 0 && channelName.find("&") != 0) {
		replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
		return -1;
	}
	Channel *channelToInvite = findChannelByName(channelName);
	if (!channelToInvite)
	{
		replyErrToClient(ERR_NOSUCHCHANNEL, userSender->getNickName(), channelName, sd, "");
		return (-1);
	}

	if (!channelToInvite->userInChannel(userSender->getNickName()))
	{
		replyErrToClient(ERR_NOTONCHANNEL, userSender->getNickName(), channelName, sd, "");	
		return (-1);
	}

	User *userSenderChannel = channelToInvite->findUserBySd(sd);
	if (!userSenderChannel)
	{
		replyErrToClient(ERR_NOSUCHNICK, userSender->getNickName(), channelName, sd, "");
		return (-1);
	}
	if (channelToInvite->getInviteOnly() && !userSenderChannel->_isOp())
	{
		replyErrToClient(ERR_CHANOPRIVSNEEDED, userSenderChannel->getNickName(), channelName, sd, "");
		return (-1);
	}

	User *userToInvite = find_by_nickname(nickInvited);
	
	if (!userToInvite)
	{
		replyErrToClient(ERR_NOSUCHNICK, nickInvited, channelName, sd, "");
		return (-1);
	}

	if (channelToInvite->userInChannel(userToInvite->getNickName()))
	{
		replyErrToClient(ERR_USERONCHANNEL, userSenderChannel->getNickName(), channelName, sd, userToInvite->getNickName());
		return (-1);
	}
	else
	{
		channelToInvite->addInviteList(userToInvite->getNickName());
		replyServToClient(RPL_INVITING, userSenderChannel->getNickName(), sd, channelName, userToInvite->getNickName());
		std::string replyMsg = ":" + userSenderChannel->getNickName() + "!" + userSenderChannel->getUserName() + "@localhost INVITE " + userToInvite->getNickName()  + " :" + channelName + "\r\n";
		send(userToInvite->sd, replyMsg.c_str(), replyMsg.length(), MSG_NOSIGNAL);
	}
	return (0);
}

int Server::mode(std::string msg, int sd) {
	int argIndex = 0;
	size_t pos = msg.find("MODE ");
	if (pos != std::string::npos) msg.erase(0, pos);
	msg.erase(0, 5);
	std::string channelName = msg.substr(0, msg.find(" "));
	msg.erase(0, msg.find(" ") + 1);
	
	if (channelName.find("#") != 0 && channelName.find("&") != 0) {		
		replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
		return -1; // Not not a channel name
	}

	if (channelName.find("\r\n") != std::string::npos)
		channelName = channelName.substr(0, channelName.length() - 2);
	
	Channel* channel = findChannelByName(channelName);
	if (channel == NULL){
		replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
		return -1; // No such channel
	}

	if (channel->findUserByNickname(find_by_sd(sd)->getNickName()) == NULL){
		replyErrToClient(ERR_NOTONCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
		return -1; // User not in channel
	}

	std::string flagsStr = "-1";
	if (msg.find("+") != std::string::npos || msg.find("-") != std::string::npos){
		flagsStr = msg.substr(0, msg.find(" "));
		msg.erase(0, msg.find(" ") + 1);
		if (flagsStr.find("\r\n") != std::string::npos)
			flagsStr = flagsStr.substr(0, flagsStr.length() - 2);
	}

	// /mode #channelName 
	if (flagsStr == "-1") {
		std::string validFlags = "+";
		std::string args = "";
		std::string pw = channel->getPassword();
		int usLimit = channel->getUserLimit();
		if (pw != "") {
			validFlags += "k";
			args += pw + " ";
		}
		if (channel->getInviteOnly())
			validFlags += "i";
		if (channel->getTopicChangeOnlyOp())
			validFlags += "t";
		if (usLimit != -1) {
			validFlags += "l";
			std::stringstream ss;
			ss << usLimit;
			args += ss.str() + " ";
		}
		std::string fin = validFlags;
		if (args != "")
			fin += " " + args;
		replyServToClient(RPL_CHANNELMODEIS, find_by_sd(sd)->getNickName(), sd, channelName, fin);
		return (0);
	}

	if (channel->findUserByNickname(find_by_sd(sd)->getNickName())->_isOp() == false){
		replyErrToClient(ERR_CHANOPRIVSNEEDED, find_by_sd(sd)->getNickName(), channelName, sd, "");
		return -1; // User is not operator
	}

	flags modeFlags;
	modeFlags.i.flag = 0;
	modeFlags.i.arg = "";

	modeFlags.t.flag = 0;
	modeFlags.t.arg = "";

	modeFlags.k.flag = 0;
	modeFlags.k.arg = "";

	modeFlags.o.flag = 0;
	modeFlags.o.arg = "";

	modeFlags.l.flag = 0;
	modeFlags.l.arg = "";

	modeFlags.setting = 0;

	std::vector<std::string> args;

	for(size_t i = 0; i < msg.length(); ) {
		if (msg[i] == ' ') {
			i++;
			continue;
		}
		size_t nextSpace = msg.find(' ', i);
		if (nextSpace == std::string::npos) {
			if (msg.find("\r\n", i) != std::string::npos)
				args.push_back(msg.substr(i, msg.find("\r\n", i) - i));
			else
				args.push_back(msg.substr(i));
			break;
		} else {
			args.push_back(msg.substr(i, nextSpace - i));
			i = nextSpace;
			while (i < msg.length() && msg[i] == ' ') {
				i++;
			}
		}
	}

	// for(size_t i = 0; i < args.size(); i++) {
	// 	std::cout << "Arg " << i << ": " << args[i] << std::endl;
	// }

	std::cout << flagsStr << std::endl;
	for (size_t i = 0; i < flagsStr.length(); i++) {
		if (flagsStr[i] == '+')
			modeFlags.setting = 1;
		else if (flagsStr[i] == '-')
			modeFlags.setting = 2;
		else if (modeFlags.setting == 0)
			return -1; // No + or - before flags
		else if (modeFlags.setting == 1) { // Adding flags

			if (flagsStr[i] == 'i')
				modeFlags.i.flag = 1;
			else if (flagsStr[i] == 't')
				modeFlags.t.flag = 1;
			else if (flagsStr[i] == 'k'){
				modeFlags.k.arg = args[argIndex];
				argIndex++;
				modeFlags.k.flag = 1;
			}
			else if (flagsStr[i] == 'o'){
				modeFlags.o.arg = args[argIndex];
				argIndex++;
				modeFlags.o.flag = 1;
			}
			else if (flagsStr[i] == 'l'){
				modeFlags.l.arg = args[argIndex];
				argIndex++;
				modeFlags.l.flag = 1;
			}
			else
			{
				std::stringstream ss;
				ss << flagsStr[i];
				replyErrToClient(ERR_UNKNOWNMODE, find_by_sd(sd)->getNickName(), "", sd, ss.str());
				return -1; // Unknown flag
			}
		}
		else if (modeFlags.setting == 2) { // Removing flags
			if (flagsStr[i] == 'i')
				modeFlags.i.flag = 2;
			else if (flagsStr[i] == 't')
				modeFlags.t.flag = 2;
			else if (flagsStr[i] == 'k')
				modeFlags.k.flag = 2;
			else if (flagsStr[i] == 'o'){
				modeFlags.o.flag = 2;
				modeFlags.o.arg = args[argIndex];
				argIndex++;
			}
			else if (flagsStr[i] == 'l')
				modeFlags.l.flag = 2;
			else
				return -1; // Unknown flag
		}
		
		// if (modeFlags.i.flag == 1)
		// 	channel->setInviteOnly(true);
		// else if (modeFlags.i.flag == 2)
		// 	channel->setInviteOnly(false);
		
		// if (modeFlags.t.flag == 1) {
		// 	// Implement topic restriction logic if needed
		// }

		// if (modeFlags.k.flag == 1) {
		// 	// Implement password logic if needed
		// }

		if (modeFlags.o.flag == 1) {
			//std::cout << "Giving operator status to |" << modeFlags.o.arg << "|" << std::endl;
			User* userToOp = channel->findUserByNickname(modeFlags.o.arg);	
			if (userToOp != NULL) {
				userToOp->SetOp(true);
				std::string opMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " +o " + modeFlags.o.arg + "\r\n";
				std::vector<User> users = channel->getUsers();
				for (size_t j = 0; j < users.size(); j++) {
					int fd = users[j].sd;
					send(fd, opMsg.c_str(), opMsg.length(), MSG_NOSIGNAL);
				}
			} else {
				replyErrToClient(ERR_NOSUCHNICK, find_by_sd(sd)->getNickName(), channelName, sd, "");
				return -1; // No such user to op
			}
		} else if (modeFlags.o.flag == 2) {
			User* userToDeop = channel->findUserByNickname(modeFlags.o.arg);
			if (userToDeop != NULL) {
				if (channel->count_operators() == 1 && userToDeop->_isOp()){
					std::vector<User> users = channel->getUsers();
					std::string partMsg = ":" + userToDeop->getNickName() + "!" + userToDeop->getNickName() + "@localhost PART " + channelName + " :" + "" + "\r\n";
					for (size_t j = 0; j < users.size(); j++) {
						int fd = users[j].sd;
						send(fd, partMsg.c_str(), partMsg.length(), MSG_NOSIGNAL);
					}
					channel->removeUser(userToDeop->getNickName());
				}
				else {
					userToDeop->SetOp(false);
					std::string deopMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " -o " + modeFlags.o.arg + "\r\n";
					std::vector<User> users = channel->getUsers();
					for (size_t j = 0; j < users.size(); j++) {
						int fd = users[j].sd;
						send(fd, deopMsg.c_str(), deopMsg.length(), MSG_NOSIGNAL);
					}
				}
			} else {
				replyErrToClient(ERR_NOSUCHNICK, find_by_sd(sd)->getNickName(), channelName, sd, "");
				return -1; // No such user to deop
			}
		}

		if (modeFlags.l.flag == 1) {
			int limit = 0;
			if (!msg.empty()) {
				limit = std::atol(msg.c_str());
				if (limit <= 0)
					return -1; // Invalid limit
				channel->setUserLimit(limit);
				std::string opMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " +l " + modeFlags.l.arg + "\r\n";
				std::vector<User> users = channel->getUsers();
				for (size_t j = 0; j < users.size(); j++) {
					int fd = users[j].sd;
					send(fd, opMsg.c_str(), opMsg.length(), MSG_NOSIGNAL);
				}
			} else {
				replyErrToClient(ERR_NEEDMOREPARAMS, find_by_sd(sd)->getNickName(), channelName, sd, "");
				return -1; // No limit provided
			}
		} else if (modeFlags.l.flag == 2 && channel->getUserLimit() != -1) {
			std::string deopMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " -l " + modeFlags.l.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, deopMsg.c_str(), deopMsg.length(), MSG_NOSIGNAL);
			}
			channel->setUserLimit(-1); // No limit
		}

		if (modeFlags.k.flag == 1) {
			channel->setPassword(modeFlags.k.arg);
			std::string opMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " +k " + modeFlags.k.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, opMsg.c_str(), opMsg.length(), MSG_NOSIGNAL);
			}
		} else if (modeFlags.k.flag == 2 && channel->getPassword() != "") {
			channel->setPassword("");
			std::string dopMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " -k " + modeFlags.k.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, dopMsg.c_str(), dopMsg.length(), MSG_NOSIGNAL);
			}
		}

		if (modeFlags.i.flag == 1 && !channel->getInviteOnly()) {
			channel->setInviteOnly(true);
			std::string opMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " +i " + modeFlags.i.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, opMsg.c_str(), opMsg.length(), MSG_NOSIGNAL);
			}
		} else if (modeFlags.i.flag == 2 && channel->getInviteOnly()) {
			channel->setInviteOnly(false);
			std::string dopMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " -i " + modeFlags.i.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, dopMsg.c_str(), dopMsg.length(), MSG_NOSIGNAL);
			}
		}

		if (modeFlags.t.flag == 1 && !channel->getTopicChangeOnlyOp()) {
			channel->setTopicChangeOnlyOp(true);
			std::string opMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " +t " + modeFlags.t.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, opMsg.c_str(), opMsg.length(), MSG_NOSIGNAL);
			}
		} else if (modeFlags.t.flag == 2 && channel->getTopicChangeOnlyOp()) {
			channel->setTopicChangeOnlyOp(false);
			std::string dopMsg = ":" + find_by_sd(sd)->getNickName() + "!" + find_by_sd(sd)->getUserName() + "@localhost MODE " + channelName + " -t " + modeFlags.t.arg + "\r\n";
			std::vector<User> users = channel->getUsers();
			for (size_t j = 0; j < users.size(); j++) {
				int fd = users[j].sd;
				send(fd, dopMsg.c_str(), dopMsg.length(), MSG_NOSIGNAL);
			}
		}
	}

	// std::cout << channelName << " mode flags set: "
	// 		  << "i=" << modeFlags.i.flag << ", "
	// 		  << "t=" << modeFlags.t.flag << ", "
	// 		  << "k=" << modeFlags.k.flag << ", "
	// 		  << "o=" << modeFlags.o.flag << ", "
	// 		  << "l=" << modeFlags.l.flag << std::endl;
	return 0;
}

int Server::parse_msg(int sd) {
    if (sd < 0)
        return -1;
    
    std::string msg(serverdata.msg);

	if (msg.find("CAP LS 302") != std::string::npos) {
		msg.erase(0, 12);
		return 0;
	}

    if (parse_entry(msg, sd) == -72) {
        return -72;
	}

	if (!find_by_sd(sd)->authenticated) {
		// replyErrToClient(ERR_PASSWDMISMATCH, "", "", sd, "");
		return -1;
	}

    if (msg.find("USER ") != std::string::npos && find_by_sd(sd)->getUserName() == "") {
        create_user(msg, sd);
        return 0;
    }

    if (msg.find("QUIT ") != std::string::npos) {
        for (size_t i = 0; i < allChannels.size(); i++) {
			if (allChannels[i].userInChannel(find_by_sd(sd)->getNickName()))
            	allChannels[i].removeUser(find_by_sd(sd)->getNickName());
		}
		remove_user(sd);
		close(sd);
        return 0;
    }

    if (msg.find("JOIN ") != std::string::npos) {
		//std::cout << msg << std::endl;
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
		if (channels.find(" ") != std::string::npos || channels.find(7) != std::string::npos) {
			if (passwords.empty() && (passwords.find(" ") != std::string::npos || passwords.find(7) != std::string::npos)) {
				replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channels, sd, "");
				return -1;
			}
		}
		std::string channelsCopy = channels;
		User tempUser = *find_by_sd(sd);
		do {
			std::string passwordsCopy = passwords;
			pos = channels.find_first_of(",");
			std::string channelName;;
			if (pos != std::string::npos)
			{
				channelName = channels.substr(0, pos);
				channels.erase(0, pos + 1);
			}
			else
				channelName = channels;
			if ((channelName.find("#") != 0 && channelName.find("&") != 0) || channelName.length() > 200 || channelName.length() < 1) {
				replyErrToClient(ERR_NOSUCHCHANNEL, find_by_sd(sd)->getNickName(), channelName, sd, "");
				return -1;
			}
			Channel channel(channelName, this);
			if (findChannelByName(channel.getChannelName()) != NULL) {
				Channel* existingChannel = findChannelByName(channel.getChannelName());
				
				// Controlli limiti e invite...
				if (existingChannel->getUserLimit() > 0 && (int)existingChannel->getUsers().size() + 1 > existingChannel->getUserLimit()) {
					replyErrToClient(ERR_CHANNELISFULL, find_by_sd(sd)->getNickName(), channelName, sd, "");
					return -1;
				}
				if (existingChannel->getInviteOnly() && !existingChannel->nickInInviteList(find_by_sd(sd)->getNickName())) {
					replyErrToClient(ERR_INVITEONLYCHAN, "", existingChannel->getChannelName(), sd, " :Cannot join channel (+i)");
					return -1;
				}
				std::vector<User> users = existingChannel->getUsers();
				if (alreadyInChannel(find_by_sd(sd)->getNickName(), existingChannel->getChannelName())) {
					continue ;
				}
				if(checkUserPassword(existingChannel, channelsCopy, passwordsCopy) == false)
				{
					replyErrToClient(ERR_BADCHANNELKEY, find_by_sd(sd)->getNickName(), channel.getChannelName(), sd, "");
					continue ; // Usa continue, non return, per provare il prossimo canale nel loop
				}

				// AGGIUNTA UTENTE (Senza loop, senza return -1 preventivo)
				if (!existingChannel->userInChannel(find_by_sd(sd)->getNickName())) {
					existingChannel->addUser(&tempUser);
				}
			}
			else if (findChannelByName(channel.getChannelName()) == NULL)
			{
				if (!assignPasswordToChannel(channelsCopy, passwordsCopy, &channel))
				{
					replyErrToClient(ERR_NEEDMOREPARAMS, find_by_sd(sd)->getNickName(), "", sd, ":There are more passwords than channels");
					return -1;
				}
				allChannels.push_back(channel);
				allChannels.back().addUser(&tempUser);
			}
		}
		while (pos != std::string::npos);
    }

    else if (msg.find("PRIVMSG ") != std::string::npos) {
        size_t pos = msg.find("PRIVMSG ");
        if (pos != std::string::npos) 
        {
            msg.erase(0, pos + 8);
			if (msg.find("BalatroBot") != std::string::npos){
				msg.erase(0, msg.find("BalatroBot") + 12);
				findBalatroBySd(sd)->getMessagePrompt(msg);
				return 0;
			}
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

    else if (msg.find("INVITE ") != std::string::npos) {
        return (invite(msg, sd));
    }

	else if (msg.find("MODE ") != std::string::npos) {
		return mode(msg, sd);
	}
    
	else if (msg.find("TOPIC ") != std::string::npos) {
        return (topic(msg, sd));
    }

    else if (msg.find("KICK ") != std::string::npos) {
        return (kick(msg, sd));
    }
    if (msg.find("printusers") != std::string::npos) {
        print_users();
        return 0;
    }

	if (msg.find("balatro") != std::string::npos) {
		User *u = find_by_sd(sd);
		if (u) {
			addBalatroBot(sd, u); // Passi direttamente il puntatore
		}
		if (findBalatroBySd(sd) != NULL)
			findBalatroBySd(sd)->startNewGame();
	}

    return 0;
}