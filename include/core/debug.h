#ifndef CORE_DEBUG_H_
#define CORE_DEBUG_H_

#include "lib/ansi.h"

#define YIKES(x) \
	(ansi::ansistream() << ansi::red << "!! " << ansi::reset << x << std::endl)

#endif
