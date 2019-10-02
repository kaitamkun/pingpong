#ifndef PINGPONG_EVENTS_EVENT_H_
#define PINGPONG_EVENTS_EVENT_H_

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "pingpong/core/channel.h"
#include "pingpong/core/debug.h"
#include "pingpong/core/local.h"
#include "pingpong/core/util.h"
#include "pingpong/core/server.h"
#include "pingpong/core/user.h"

namespace pingpong {
	class event {
		protected:
			bool is_empty = false;
			event(bool empty, const std::string &content_): is_empty(empty), content(content_) {}

		public:
			long stamp = util::timestamp();
			std::string content;

			virtual ~event() = default;
			event(): is_empty(true) {}

			operator bool() const { return !is_empty; }
	};

	using listener_fn = std::function<void(event *)>;

	class fake_event: public event { public: fake_event(): event() {} };

	class events {
		private:
			static std::multimap<std::string, listener_fn> listeners;

		public:
			template <typename T>
			static void listen(std::function<void(T *)> fn) {
				listeners.insert(std::pair<std::string, listener_fn>(
					std::string(typeid(T).name()),
					[=](event *ev) {
						fn(dynamic_cast<T *>(ev));
					}
				));
			}

			template <typename T>
			static void dispatch(T *ptr) {
				auto range = listeners.equal_range(typeid(T).name());
				for (auto i = range.first; i != range.second; ++i)
					i->second(ptr);
			}

			template <typename T>
			static void dispatch(std::unique_ptr<T> &&ptr) {
				dispatch(ptr.get());
			}

			template <typename T, typename... Args>
			static void dispatch(Args && ...args) {
				dispatch<T>(std::make_unique<T>(std::forward<Args>(args)...));
			}
	};

	// For events local to one server.
	struct server_event: public event {
		server *serv;

		server_event(server *serv_, const std::string &content_ = ""): event(false, content_), serv(serv_) {}
	};

	/** For events local to one channel on one server. */
	struct channel_event: public server_event {
		std::shared_ptr<channel> chan;

		channel_event(const std::shared_ptr<channel> &, server *, const std::string & = "");
		channel_event(const std::shared_ptr<channel> &chan_, const std::string &content_ = ""):
			channel_event(chan_, chan_? chan_->serv : nullptr, content_) {}
	};

	/** For events local to one user in one channel on one server, such as joins.
	 *  This can also be used for things like quits, which are specific to a user and server but not to a channel, by
	 *  leaving the channel pointer null. */
	struct user_event: public channel_event {
		std::shared_ptr<user> who;

		user_event(const std::shared_ptr<user> &, const std::shared_ptr<channel> &, const std::string & = "");
		user_event(const std::shared_ptr<user> &who_, const std::string &content_ = ""):
			user_event(who_, nullptr, content_) {}
	};

	/** For events local on one server to either a user or a channel, such as privmsgs. */
	struct local_event: public server_event, public local {
		template <typename T>
		local_event(server *serv_, const T &where_, const std::string &content_ = ""):
			server_event(serv_, content_), local(where_) {}
	};

	/** For events local to two users in one channel on one server, such as kicks. */
	struct targeted_event: public user_event {
		std::shared_ptr<user> whom;

		targeted_event(const std::shared_ptr<user> &, const std::shared_ptr<user> &,
			const std::shared_ptr<channel> &, const std::string & = "");

		targeted_event(std::shared_ptr<user> who_, const std::shared_ptr<user> &whom_, server *serv_,
		const std::string &content_ = ""):
			targeted_event(who_, whom_, static_cast<std::shared_ptr<channel>>(nullptr), content_) { serv = serv_; }
	};
}

#endif