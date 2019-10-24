#ifndef PINGPONG_EVENTS_PRIVMSG_H_
#define PINGPONG_EVENTS_PRIVMSG_H_

#include "pingpong/core/channel.h"
#include "pingpong/core/local.h"
#include "pingpong/core/user.h"
#include "pingpong/events/event.h"

namespace pingpong {
	/**
	 * Represents a message to a user or a channel.
	 */
	struct privmsg_event: public local_event {
		std::shared_ptr<user> speaker;

		template <typename T>
		privmsg_event(const std::shared_ptr<user> &speaker_, const T &where_, const std::string &message_):
			local_event(speaker_->serv, where_, message_), speaker(speaker_) {}
	};
}

#endif
