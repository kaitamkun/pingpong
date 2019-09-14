#ifndef PINGPONG_EVENTS_NICK_H_
#define PINGPONG_EVENTS_NICK_H_

#include "core/channel.h"
#include "events/event.h"

namespace pingpong {
	/**
	 * Represents a nick change. In this event, the user_ptr points to the user after their nick has been updated, while
	 * the content contains the old nickname.
	 */
	class nick_event: public user_event {
		public:
			using user_event::user_event;
	};
}

#endif
