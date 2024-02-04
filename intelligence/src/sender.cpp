#include <easylogging/easylogging++.h>
#include "run/logger.hpp"

int main(int argc, char** argv) {
	START_EASYLOGGINGPP(argc, argv);
	auto logger_conf = run::log::get_config();
	el::Loggers::reconfigureLogger("default", logger_conf);
}
