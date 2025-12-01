#include "Channel.hpp"

Channel::Channel() {
	channel_name = "";
	topic = "";
}

Channel::~Channel() {
	users.clear();
}

Channel::Channel(std::string name, Server *serv) {
    topic = "";
    server = serv;

    if (name.length() > 200 || name.empty()) {
        channel_name = "invalid";
        return;
    }

    if (name[0] != '#' && name[0] != '&') {
        channel_name = "#" + name;
    } else {
        channel_name = name;
    }
}

Channel::Channel(const Channel &other) {
	channel_name = other.channel_name;
	users = other.users;
	server = other.server;
}

Channel &Channel::operator=(const Channel &other) {
	if (this != &other)
	{
		channel_name = other.channel_name;
		users = other.users;
		server = other.server;
	}
    return *this;
}

std::string Channel::getChannelName() {
	return channel_name;
}

std::vector<User> Channel::getUsers() {
	return users;
}

void Channel::addUser(User *user) {
	users.push_back(*user);
	std::cout << "User " << users[0].getNickName() << std::endl;
	std::string joinMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@127.0.0.1 JOIN :" + channel_name + "\r\n";
    if (users.size() == 1)
			users.back().SetOp(true);
	send(user->sd, joinMsg.c_str(), joinMsg.length(), 0);
    for (size_t i = 0; i < users.size(); i++) {
        int fd = users[i].sd;
        send(fd, joinMsg.c_str(), joinMsg.length(), 0);
    }

	if (!this->topic.empty()) {
        std::string topicMsg = ":localhost 332 " + user->getNickName() + " " + channel_name + " :" + this->topic + "\r\n";
        send(user->sd, topicMsg.c_str(), topicMsg.length(), 0);
    }

    std::string namesList = "";
    for (size_t i = 0; i < users.size(); i++) {
		if (users[i]._isOp())
			namesList += "@";
        namesList += users[i].getNickName() + " ";
    }

	std::cout << "Names list: " << namesList << std::endl;
	
	server->replyServToClient(RPL_NAMREPLY, user->getNickName(), user->sd, channel_name, namesList);
    // std::string nameReply = ":localhost 353 " + user->getNickName() + " = " + channel_name + " :" + namesList + "\r\n";
	// send(user->sd, nameReply.c_str(), nameReply.length(), 0);

	// Formato 366: :Server 366 Nick #canale :End of /NAMES list
	server->replyServToClient(RPL_ENDOFNAMES, user->getNickName(), user->sd, channel_name, "End of /NAMES list");
	// std::string endNames = ":localhost 366 " + user->getNickName() + " " + channel_name + " :End of /NAMES list\r\n";
	// send(user->sd, endNames.c_str(), endNames.length(), 0);
}

User *Channel::findUserByNickname(std::string nickname) {
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i].getNickName() == nickname) {
			return &users[i];
		}
	}
	return NULL;
}

int Channel::removeUser(std::string nickname) {
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i].getNickName() == nickname) {
			users.erase(users.begin() + i);
			return 1;
		}
	}
	return 0;
}

bool	Channel::userInChannel(std::string nickname)
{
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getNickName() == nickname)
			return (true);
	}
	return (false);
}