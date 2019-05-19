#ifndef CORE_IRC_H_
#define CORE_IRC_H_

namespace pingpong { class server; }

#include <vector>

namespace pingpong {
	class irc {
		private:
			std::vector<server> servers;

		public:
			static constexpr int default_port = 6667;

			
	};
}

#endif