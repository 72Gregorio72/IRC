#include "Channel.hpp"

Channel::Channel() {
	channel_name = "";
	topic = "";
}

Channel::~Channel() {
	users.clear();
}

Channel::Channel(std::string name) {
	if (channel_name[0] != '#' && channel_name[0] != '&'){
		channel_name = "#" + channel_name;
	}
	if (channel_name.length() > 200) {
		std::cout << "Error: Channel name too long." << std::endl;
		return ;
	}
	channel_name = name;
	allChannels.push_back(*this);
	topic = "";
}

Channel::Channel(const Channel &other) {
	channel_name = other.channel_name;
	users = other.users;
}

Channel &Channel::operator=(const Channel &other) {
	if (this != &other)
	{
		channel_name = other.channel_name;
		users = other.users;
	}
    return *this;
}

std::string Channel::getChannelName() {
	return channel_name;
}

std::vector<User> Channel::getUsers() {
	return users;
}

void Channel::addUser(User user) {
	users.push_back(user);
	std::string joinMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@127.0.0.1 JOIN :" + channel_name + "\r\n";
    
    for (size_t i = 0; i < users.size(); i++) {
        int fd = users[i].sd;
        send(fd, joinMsg.c_str(), joinMsg.length(), 0);
    }

	if (!this->topic.empty()) {
        std::string topicMsg = ":127.0.0.1 332 " + user.getNickName() + " " + channel_name + " :" + this->topic + "\r\n";
        send(user.sd, topicMsg.c_str(), topicMsg.length(), 0);
    }

    std::string namesList = "";
    for (size_t i = 0; i < users.size(); i++) {
        // Qui dovresti controllare se l'utente è operatore per aggiungere "@"
        // Esempio: if (users[i].isOp()) namesList += "@";
        namesList += users[i].getNickName() + " ";
    }
	
    std::string nameReply = ":127.0.0.1 353 " + user.getNickName() + " = " + channel_name + " :" + namesList + "\r\n";
	send(user.sd, nameReply.c_str(), nameReply.length(), 0);

	// Formato 366: :Server 366 Nick #canale :End of /NAMES list
	std::string endNames = ":127.0.0.1 366 " + user.getNickName() + " " + channel_name + " :End of /NAMES list\r\n";
	send(user.sd, endNames.c_str(), endNames.length(), 0);
}

void Channel::removeUser(std::string nickname) {
	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it) {
		if (it->getNickName() == nickname) {
			users.erase(it);
			break;
		}
	}
}

Channel* Channel::findChannelByName(std::string name) {
	for (size_t i = 0; i < allChannels.size(); i++) {
		if (allChannels[i].getChannelName() == name) {
			return &allChannels[i];
		}
	}
	return NULL;
}
