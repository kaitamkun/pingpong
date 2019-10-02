#ifndef MESSAGES_QUIT_H_
#define MESSAGES_QUIT_H_

#include <string>

#include "message.h"
#include "sourced.h"

namespace pingpong {
	class quit_message: public sourced_message {
		public:
			using sourced_message::sourced_message;

			static constexpr auto get_name = []() -> std::string { return "QUIT"; };

			operator std::string() const override;
			bool operator()(server *) override;
	};
}

#endif