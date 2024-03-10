#pragma once

namespace core::extensions::remote::uart {
	int open_serial(const char* t_port, int baudrate);
}
