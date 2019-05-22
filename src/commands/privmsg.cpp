#include <string>

#include "commands/privmsg.h"

namespace pingpong {
	privmsg_command::operator std::string() const {
		return "PRIVMSG " + destination.name + " :" + message;
	}
}