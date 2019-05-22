#include <memory>
#include <stdexcept>
#include <string>

#include "messages/all.h"
#include "core/debug.h"

namespace pingpong {
	std::map<std::string, message_ctor> message::ctors;

	message_ptr message::parse(const pingpong::line &line) {
		if (numeric_message::is_numeric(line.command.c_str()))
			return std::make_unique<pingpong::numeric_message>(numeric_message(line));

		auto ctor = message::ctors.find(line.command);
		if (ctor == message::ctors.end())
			throw std::invalid_argument("Unknown message");

		return ctor->second(line);
	}

	message::~message() = default;

	message::operator std::string() const {
		return std::string(line);
	}
}