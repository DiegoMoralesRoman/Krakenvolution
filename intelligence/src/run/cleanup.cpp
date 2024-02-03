#include "run.hpp"

#include <easylogging/easylogging++.h>
#include <google/protobuf/stubs/common.h>

void run::shutdown_application() {
	LOG(INFO) << "󰍛 Freeing memory...";
	google::protobuf::ShutdownProtobufLibrary();
}
