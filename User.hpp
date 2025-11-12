#ifndef USER_HPP
#define USER_HPP

#include "ft_irc.h"

class User{
	public:
		User();
		~User();
		User(const User &other);
		User &operator=(const User &other);
	private:
		std::string User_name;
		bool isOp;
};

#endif