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
				return -72;
			}
		}

		if (size_t pos = msg.find("NICK ") != std::string::npos && find_by_sd(sd)->getNickName() == "" && find_by_sd(sd)->authenticated) {
			if (pos != std::string::npos)
				msg.erase(0, pos);
			msg.erase(0, 4);
			std::string nickname = msg.substr(0, msg.find_first_of("\n"));
			if (find_by_nickname(nickname) != NULL) {
				reply_to_user(ERR_NICKNAMEINUSE, nickname, sd);
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
		Channel channel(msg);
		if (findChannelByName(channel.getChannelName()) != NULL) {
			Channel* existingChannel = findChannelByName(channel.getChannelName());
			existingChannel->addUser(find_by_sd(sd));
			return 0;
		}
		allChannels.push_back(channel);
		allChannels.back().addUser(find_by_sd(sd));
	}
	return 0;
}