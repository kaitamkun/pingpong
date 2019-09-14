#ifndef PINGPONG_NET_SOCKET_BUFFER_H_
#define PINGPONG_NET_SOCKET_BUFFER_H_

#include <iostream>

#include "net/sock.h"

namespace pingpong::net {
	class socket_buffer: public std::streambuf {
		private:
			sock *source;
		
		protected:
			virtual std::streamsize xsputn(const char *, std::streamsize) override;
			virtual std::streamsize xsgetn(char *, std::streamsize) override;
			virtual std::streambuf::int_type overflow(std::streambuf::int_type) override;
			virtual std::streambuf::int_type underflow() override;

		public:
			socket_buffer(sock *source_): source(source_) {}
	};
}

#endif