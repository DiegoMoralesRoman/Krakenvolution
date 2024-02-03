#pragma once

#include<netinet/in.h>

namespace core::extensions::remote::sources::tcp {
	struct Context {
		int sock_fd;
		sockaddr_in addr;
	};
}
