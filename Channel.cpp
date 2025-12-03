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
	std::string joinMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@localhost JOIN :" + channel_name + "\r\n";
    if (users.size() == 1)
			users.back().SetOp(true);
	send(user->sd, joinMsg.c_str(), joinMsg.length(), MSG_NOSIGNAL);
    for (size_t i = 0; i < users.size(); i++) {
        int fd = users[i].sd;
        send(fd, joinMsg.c_str(), joinMsg.length(), MSG_NOSIGNAL);
    }

	if (!this->topic.empty()) {
        std::string topicMsg = ":localhost 332 " + user->getNickName() + " " + channel_name + " :" + this->topic + "\r\n";
        send(user->sd, topicMsg.c_str(), topicMsg.length(), MSG_NOSIGNAL);
    }

    std::string namesList = "";
    for (size_t i = 0; i < users.size(); i++) {
		if (users[i]._isOp())
			namesList += "@";
        namesList += users[i].getNickName() + " ";
    }

	std::cout << "Names list: " << namesList << std::endl;
	
	server->replyServToClient(RPL_NAMREPLY, user->getNickName(), user->sd, channel_name, namesList);

	server->replyServToClient(RPL_ENDOFNAMES, user->getNickName(), user->sd, channel_name, "End of /NAMES list");
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
            bool isOp = users[i]._isOp();
            std::string channel_name = this->getChannelName();

            if (isOp && count_operators() > 1) {
                users[i].SetOp(false);
            } else if (isOp && count_operators() == 1) {
				for (size_t j = 0; j < users.size(); j++) {
					std::string partMsg = ":" + users[j].getNickName() + "!" + users[j].getUserName() + "@localhost PART " + channel_name + " :Channel closed by operator\r\n";
					for (size_t k = 0; k < users.size(); ++k) {
						if (k == i) continue;
							send(users[k].sd, partMsg.c_str(), partMsg.length(), MSG_NOSIGNAL);
					}
				}
				users.erase(users.begin() + i);
				server->deleteChannel(channel_name);
				return 1;
            }
            std::string msgToSend = ":" + users[i].getNickName() + "!" + users[i].getUserName() + "@localhost PART " + channel_name + " :Leaving\r\n";
            std::vector<User> usersInChannel = getUsers();
            for (size_t k = 0; k < usersInChannel.size(); k++) {
				if (usersInChannel[k].getNickName() != nickname)
                	send(usersInChannel[k].sd, msgToSend.c_str(), msgToSend.length(), MSG_NOSIGNAL);
            }

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

int	Channel::count_operators()
{
	int count = 0;
	for (size_t i = 0; i < users.size(); i++) {
		if (users[i]._isOp())
			count++;
	}
	return (count);
}