#ifndef PINGPONG_MESSAGES_MODE_H_
#define PINGPONG_MESSAGES_MODE_H_

#include "pingpong/core/local.h"
#include "pingpong/core/modeset.h"

#include "pingpong/messages/message.h"

namespace pingpong {
	/**
	 * Received whenever someone changes the mode in a channel or when your usermode is changed.
	 */
	class mode_message: public message, public local {
		public:
			modeset mset;

			std::shared_ptr<user> who;

			/** If the mode change is for a channel, this stores a pointer to the channel. */
			std::shared_ptr<pingpong::channel> chan;

			mode_message(const pingpong::line &);

			static constexpr auto get_name = []() -> std::string { return "MODE"; };
			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif